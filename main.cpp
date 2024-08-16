#include "crow_all.h"
#include <vector>
#include <string>
#include <mutex>

struct Product {
    int id;
    std::string name;
    std::string description;
    double price;
    int quantity;
    std::string image_url;

    CROW_JSON_SERIALIZE(id, name, description, price, quantity, image_url);
};

std::vector<Product> products;
std::mutex products_mutex;

int main() {
    crow::SimpleApp app;

    // Get all products
    CROW_ROUTE(app, "/products")
    ([]() {
        std::lock_guard<std::mutex> lock(products_mutex);
        return crow::json::wvalue(products);
    });

    // Get a product by ID
    CROW_ROUTE(app, "/products/<int>")
    ([](int id) {
        std::lock_guard<std::mutex> lock(products_mutex);
        for (const auto& product : products) {
            if (product.id == id) {
                return crow::response(crow::json::wvalue(product));
            }
        }
        return crow::response(404, "Product not found");
    });

    // Create a new product
    CROW_ROUTE(app, "/products").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(400, "Invalid JSON");
        }

        Product new_product{
            (int)products.size() + 1,
            x["name"].s(),
            x["description"].s(),
            x["price"].d(),
            x["quantity"].i(),
            x["image_url"].s()
        };

        std::lock_guard<std::mutex> lock(products_mutex);
        products.push_back(new_product);

        return crow::response(201, crow::json::wvalue(new_product));
    });

    // Update an existing product
    CROW_ROUTE(app, "/products/<int>").methods(crow::HTTPMethod::PUT)
    ([](const crow::request& req, int id) {
        auto x = crow::json::load(req.body);
        if (!x) {
            return crow::response(400, "Invalid JSON");
        }

        std::lock_guard<std::mutex> lock(products_mutex);
        for (auto& product : products) {
            if (product.id == id) {
                product.name = x["name"].s();
                product.description = x["description"].s();
                product.price = x["price"].d();
                product.quantity = x["quantity"].i();
                product.image_url = x["image_url"].s();
                return crow::response(200, crow::json::wvalue(product));
            }
        }
        return crow::response(404, "Product not found");
    });

    // Delete a product
    CROW_ROUTE(app, "/products/<int>").methods(crow::HTTPMethod::DELETE)
    ([](int id) {
        std::lock_guard<std::mutex> lock(products_mutex);
        auto it = std::remove_if(products.begin(), products.end(), [id](const Product& product) {
            return product.id == id;
        });

        if (it != products.end()) {
            products.erase(it, products.end());
            return crow::response(204);
        } else {
            return crow::response(404, "Product not found");
        }
    });

    // Start the server
    app.port(8080).multithreaded().run();
}

