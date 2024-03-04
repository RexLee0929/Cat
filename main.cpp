#include "crow.h"
#include <sstream>
#include <memory>
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>



int main() {
    crow::SimpleApp app;


//register
CROW_ROUTE(app, "/api/register").methods("POST"_method)([](const crow::request& req){
    auto x = crow::json::load(req.body);
    if (!x) {
        crow::response res(400);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["error"] = "请求格式错误";
        res.body = crow::json::dump(dto);
        return res;
    }
    
    std::string username = x["username"].s();
    std::string email = x["email"].s();
    std::string password = x["password"].s();

    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO users(username, email, password) VALUES (?, ?, ?)"));
        pstmt->setString(1, username);
        pstmt->setString(2, email);
        pstmt->setString(3, password);
        pstmt->executeUpdate();

        crow::response res(200);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["message"] = "注册成功";
        res.body = crow::json::dump(dto);
        return res;
    } catch (sql::SQLException& e) {
        crow::response res(500);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["error"] = "服务器错误";
        res.body = crow::json::dump(dto);
        return res;
    }
});
//login
CROW_ROUTE(app, "/api/login").methods("POST"_method)([](const crow::request& req){
    auto x = crow::json::load(req.body);
    if (!x) {
        crow::response res(400);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["error"] = "请求格式错误";
        res.body = crow::json::dump(dto);
        return res;
    }
    
    std::string username = x["username"].s();
    std::string password = x["password"].s();

    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM users WHERE username = ? AND password = ?"));
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            crow::response res(200);
            res.add_header("Content-Type", "application/json");
            crow::json::wvalue dto;
            dto["message"] = "登录成功";
            res.body = crow::json::dump(dto);
            return res;
        } else {
            crow::response res(400);
            res.add_header("Content-Type", "application/json");
            crow::json::wvalue dto;
            dto["error"] = "用户名或密码错误";
            res.body = crow::json::dump(dto);
            return res;
        }
    } catch (sql::SQLException& e) {
        crow::response res(500);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["error"] = "服务器错误";
        res.body = crow::json::dump(dto);
        return res;
    }
});
//reset_password
CROW_ROUTE(app, "/api/reset_password").methods("POST"_method)([](const crow::request& req){
    auto x = crow::json::load(req.body);
    if (!x) {
        crow::response res(400);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["error"] = "请求格式错误";
        res.body = crow::json::dump(dto);
        return res;
    }

    std::string username = x["username"].s();
    std::string email = x["email"].s();
    std::string newPassword = x["password"].s(); // 应使用加密处理

    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");

        // 首先检查用户名和邮箱是否匹配
        std::unique_ptr<sql::PreparedStatement> pstmtCheck(con->prepareStatement("SELECT * FROM users WHERE username = ? AND email = ?"));
        pstmtCheck->setString(1, username);
        pstmtCheck->setString(2, email);
        std::unique_ptr<sql::ResultSet> resCheck(pstmtCheck->executeQuery());

        if (resCheck->next()) {
            // 如果匹配，则更新密码
            std::unique_ptr<sql::PreparedStatement> pstmtUpdate(con->prepareStatement("UPDATE users SET password = ? WHERE username = ? AND email = ?"));
            pstmtUpdate->setString(1, newPassword); // 在实际应用中应加密密码
            pstmtUpdate->setString(2, username);
            pstmtUpdate->setString(3, email);
            pstmtUpdate->executeUpdate();

            crow::response res(200);
            res.add_header("Content-Type", "application/json");
            crow::json::wvalue dto;
            dto["message"] = "密码重置成功";
            res.body = crow::json::dump(dto);
            return res;
        } else {
            // 用户名和邮箱不匹配
            crow::response res(400);
            res.add_header("Content-Type", "application/json");
            crow::json::wvalue dto;
            dto["error"] = "用户名或邮箱不匹配";
            res.body = crow::json::dump(dto);
            return res;
        }
    } catch (sql::SQLException& e) {
        crow::response res(500);
        res.add_header("Content-Type", "application/json");
        crow::json::wvalue dto;
        dto["error"] = "服务器错误";
        res.body = crow::json::dump(dto);
        return res;
    }
});

//rescue-request
CROW_ROUTE(app, "/api/submit_rescue_request").methods("POST"_method)([](const crow::request& req, crow::response& res) {
    auto x = crow::json::load(req.body);
    if (!x) {
        crow::json::wvalue errorDto;
        errorDto["error"] = "请求格式错误";
        res.code = 400;
        res.write(crow::json::dump(errorDto));
        res.end();
        return;
    }

    std::string user_only;
    std::unique_ptr<sql::Connection> con;
    
    try {
        auto driver = sql::mysql::get_driver_instance();
        con.reset(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");
    } catch (sql::SQLException& e) {
        crow::json::wvalue errorDto;
        errorDto["error"] = "数据库连接失败: " + std::string(e.what());
        res.code = 500;
        res.write(crow::json::dump(errorDto));
        res.end();
        return;
    }

    // 检查是否需要登录
    if (x.has("isLogin") && x["isLogin"].b()) {
        std::string username = x.has("username") ? std::string(x["username"].s()) : "";
        std::string password = x.has("password") ? std::string(x["password"].s()) : "";

        try {
            // 使用预处理语句来保护免受SQL注入攻击
            std::unique_ptr<sql::PreparedStatement> pstmtVerify(con->prepareStatement("SELECT user_only FROM users WHERE username = ? AND password = ?"));
            pstmtVerify->setString(1, username);
            pstmtVerify->setString(2, password);
            std::unique_ptr<sql::ResultSet> rsVerify(pstmtVerify->executeQuery());

            if (rsVerify->next()) {
                user_only = rsVerify->getString("user_only");
            } else {
                crow::json::wvalue errorDto;
                errorDto["error"] = "用户名或密码错误";
                res.code = 401; // 更改为401未授权
                res.write(crow::json::dump(errorDto));
                res.end();
                return;
            }
        } catch (sql::SQLException& e) {
            crow::json::wvalue errorDto;
            errorDto["error"] = "登录过程中出现数据库错误: " + std::string(e.what());
            res.code = 500;
            res.write(crow::json::dump(errorDto));
            res.end();
            return;
        }
    }

    // 救助请求的详细信息
    std::string animalInfo = x["animalInfo"].s();
    int animalCount = x["animalCount"].i();
    std::string animalStatus = x["animalStatus"].s();
    std::string rescueAddress = x["rescueAddress"].s();
    std::string applicantName = x["applicantName"].s();
    std::string applicantGender = x["applicantGender"].s();
    std::string applicantPhone = x["applicantPhone"].s();
    std::string applicantCity = x["applicantCity"].s();
    std::string proofImage = x["proofImage"].s();
    std::string proofVideo = x["proofVideo"].s();
    std::string rescueOnly = x["rescue_only"].s();

    try {
        std::string sqlQuery = "INSERT INTO rescue_requests(animal_info, animal_count, animal_status, rescue_address, applicant_name, applicant_gender, applicant_phone, applicant_city, proof_image, proof_video, rescue_only";
        if (!user_only.empty()) {
            sqlQuery += ", user_only) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        } else {
            sqlQuery += ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(sqlQuery));
        int parameterIndex = 1;
        pstmt->setString(parameterIndex++, animalInfo);
        pstmt->setInt(parameterIndex++, animalCount);
        pstmt->setString(parameterIndex++, animalStatus);
        pstmt->setString(parameterIndex++, rescueAddress);
        pstmt->setString(parameterIndex++, applicantName);
        pstmt->setString(parameterIndex++, applicantGender);
        pstmt->setString(parameterIndex++, applicantPhone);
        pstmt->setString(parameterIndex++, applicantCity);
        pstmt->setString(parameterIndex++, proofImage);
        pstmt->setString(parameterIndex++, proofVideo);
        pstmt->setString(parameterIndex++, rescueOnly);
        if (!user_only.empty()) {
            pstmt->setString(parameterIndex, user_only);
        }

        pstmt->executeUpdate();

        crow::json::wvalue successDto;
        successDto["message"] = "救助请求已成功提交";
        res.code = 200;
        res.write(crow::json::dump(successDto));
    } catch (sql::SQLException& e) {
        crow::json::wvalue errorDto;
        errorDto["error"] = "提交救助请求过程中出现数据库错误: " + std::string(e.what());
        res.code = 500;
        res.write(crow::json::dump(errorDto));
    }
    res.end();
});

//vlount-application
CROW_ROUTE(app, "/api/submit_volunteer_application").methods("POST"_method)([](const crow::request& req, crow::response& res) {
    auto x = crow::json::load(req.body);
    if (!x || !x.has("username") || !x.has("password") || !x.has("volapp_Only")) {
        crow::json::wvalue dto;
        dto["error"] = "请求格式错误或缺少必要信息";
        res.body = crow::json::dump(dto);
        res.code = 400;
        res.end();
        return;
    }

    std::string username = x["username"].s();
    std::string password = x["password"].s();
    std::string name = x["name"].s();
    std::string gender = x["gender"].s();
    std::string birthdate = x["birthdate"].s();
    std::string city = x["city"].s();
    std::string phone = x["phone"].s();
    std::string reason = x["reason"].s();
    std::string Onlyvolapp = x["volapp_Only"].s();

    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");

        // 验证用户身份
        std::unique_ptr<sql::PreparedStatement> pstmtVerify(con->prepareStatement("SELECT user_only FROM users WHERE username = ? AND password = ?"));
        pstmtVerify->setString(1, username);
        pstmtVerify->setString(2, password);
        std::unique_ptr<sql::ResultSet> rsVerify(pstmtVerify->executeQuery());
        if (!rsVerify->next()) {
            crow::json::wvalue dto;
            dto["error"] = "用户验证失败";
            res.body = crow::json::dump(dto);
            res.code = 400;
            res.end();
            return;
        }
        std::string user_only = rsVerify->getString("user_only");

        // 插入志愿者申请
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO volunteer_applications(name, gender, birthdate, city, phone, reason, user_only, volapp_only) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, gender);
        pstmt->setString(3, birthdate);
        pstmt->setString(4, city);
        pstmt->setString(5, phone);
        pstmt->setString(6, reason);
        pstmt->setString(7, user_only); // 设置user_only
        pstmt->setString(8, Onlyvolapp); // 设置Onlyvolapp
        pstmt->executeUpdate();

        crow::json::wvalue dto;
        dto["message"] = "志愿者申请提交成功";
        dto["volapp_Only"] = Onlyvolapp; // 用于前端显示的唯一标识符
        res.body = crow::json::dump(dto);
        res.code = 200;
    } catch (sql::SQLException& e) {
        std::cerr << "SQLException: " << e.what() << '\n';
        crow::json::wvalue dto;
        dto["error"] = "服务器错误";
        res.body = crow::json::dump(dto);
        res.code = 500;
    }
    res.end();
});






//volunteer-task
CROW_ROUTE(app, "/api/get_rescue_tasks")([]() {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");

        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM rescue_requests WHERE is_approved = 1"));

        crow::json::wvalue dto;
        std::vector<crow::json::wvalue> tasks;

        while (res->next()) {
            crow::json::wvalue task;
            task["id"] = res->getInt("id");
            task["animal_info"] = res->getString("animal_info");
            task["animal_count"] = res->getInt("animal_count");
            task["animal_status"] = res->getString("animal_status");
            task["rescue_address"] = res->getString("rescue_address");
            task["applicant_name"] = res->getString("applicant_name");
            task["applicant_gender"] = res->getString("applicant_gender");
            task["applicant_phone"] = res->getString("applicant_phone");
            task["applicant_city"] = res->getString("applicant_city");
            task["proof_image"] = res->getString("proof_image");
            task["proof_video"] = res->getString("proof_video");
            task["assigned_volunteer"] = res->getString("assigned_volunteer");
            task["is_finished"] = res->getBoolean("is_finished");
            tasks.push_back(std::move(task));
        }

        dto["tasks"] = std::move(tasks);
        return crow::response(200, crow::json::dump(dto));
    } catch (sql::SQLException& e) {
        crow::json::wvalue errorDto;
        errorDto["error"] = "服务器错误";
        return crow::response(500, crow::json::dump(errorDto));
    }
});

//volunteer-app-rescue
CROW_ROUTE(app, "/api/volunteer_apply_rescue").methods("POST"_method)([&](const crow::request& req, crow::response& res) {
    auto x = crow::json::load(req.body);
    if (!x || !x.has("username") || !x.has("password") || !x.has("rescueOnly")) {
        crow::json::wvalue dto;
        dto["error"] = "请求结构错误或缺少必要信息";
        res.write(crow::json::dump(dto));
        res.code = 400;
        res.end();
        return;
    }

    std::string username = x["username"].s();
    std::string password = x["password"].s();
    std::string rescueOnly = x["rescueOnly"].s();

    try {
        auto driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "cat", "Aa112211."));
        con->setSchema("cat");

        // 验证志愿者身份并获取user_only和gender
        std::unique_ptr<sql::PreparedStatement> pstmtCheck(con->prepareStatement("SELECT id, user_only, gender FROM users WHERE username = ? AND password = ? AND is_volunteer = 1"));
        pstmtCheck->setString(1, username);
        pstmtCheck->setString(2, password);
        std::unique_ptr<sql::ResultSet> resCheck(pstmtCheck->executeQuery());

        if (resCheck->next()) {
            std::string userOnly = resCheck->getString("user_only");
            std::string userGender = resCheck->getString("gender");

            // 插入申请记录到等待审核表
            std::unique_ptr<sql::PreparedStatement> pstmtInsert(con->prepareStatement("INSERT INTO waiting_for_approve (app_username, app_gender, source, source_only, user_only) VALUES (?, ?, ?, ?, ?)"));
            pstmtInsert->setString(1, username);
            pstmtInsert->setString(2, userGender); // 使用从数据库获取的gender
            pstmtInsert->setString(3, "volunteer_app_rescue");
            pstmtInsert->setString(4, rescueOnly);
            pstmtInsert->setString(5, userOnly);
            pstmtInsert->executeUpdate();

            crow::json::wvalue dto;
            dto["message"] = "申请提交成功，等待审核。申请单号: " + rescueOnly;
            res.write(crow::json::dump(dto));
            res.code = 200;
        } else {
            crow::json::wvalue dto;
            dto["error"] = "登录信息错误或者志愿者身份不符合";
            res.write(crow::json::dump(dto));
            res.code = 400;
        }
    } catch (sql::SQLException& e) {
        std::cerr << "SQLException: " << e.what() << '\n';
        crow::json::wvalue dto;
        dto["error"] = "服务器错误";
        res.write(crow::json::dump(dto));
        res.code = 500;
    }
    res.end();
});



    app.port(18081).multithreaded().run();
}