#include <iostream>
#include <functional>
#include <string>
#include <set>
#include <cctype>
#include <sstream>

// 标准化表达式，去除多余的空格和括号
std::string serializeExpression(const std::string& exp) {
    std::string result;
    for (char c : exp) {
        bool inParentheses = false;
        if (c == '(' || c == ')' || std::isspace(c)) {
            inParentheses = true;
        }
        if (!inParentheses) {
            result += c;
        }
    }
    return result;
}

// 检查两个表达式的乘法操作是否相同
std::function<bool(const std::string&, const std::string&)> findmultiply = [] (const std::string& str1, const std::string& str2) -> bool {
    size_t pos1 = str1.find('*');
    size_t pos2 = str2.find('*');

    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        return false;
    }

    int a1 = std::stoi(str1.substr(pos1 - 1, 1));
    int a2 = std::stoi(str1.substr(pos1 + 1, 1));
    int b1 = std::stoi(str2.substr(pos2 - 1, 1));
    int b2 = std::stoi(str2.substr(pos2 + 1, 1));

    return (a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1);
};

// 检查两个表达式的加法操作是否相同
std::function<bool(const std::string&, const std::string&)> findplus = [] (const std::string& str1, const std::string& str2) -> bool {
    size_t pos1 = str1.find('+');
    size_t pos2 = str2.find('+');

    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        return false;
    }

    int a1 = std::stoi(str1.substr(pos1 - 1, 1));
    int a2 = std::stoi(str1.substr(pos1 + 1, 1));
    int b1 = std::stoi(str2.substr(pos2 - 1, 1));
    int b2 = std::stoi(str2.substr(pos2 + 1, 1));

    return (a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1);
};

int start[4];
bool vis[4];
std::set<std::string> seenExpressions;
std::string operation;

int main() {
    for (auto& x : start) {
        std::cin >> x;
    }

    std::function<std::string(const std::string, char, const std::string)> format = [](const std::string& op1, char op, const std::string& op2) -> std::string {
        return "(" + op1 + " " + op + " " + op2 + ")";
    };

    bool flag = false;
    std::function<void(double, int, const std::string)> dfs = [&](double ans, int x, const std::string& exp) {
        if (x == 4) {
            std::string serializedExp = serializeExpression(exp);
            bool isDuplicate = false;
            if (!seenExpressions.empty()) {
                for (const auto& seenExp : seenExpressions) {
                    if (ans == 24 && (findplus(serializedExp, serializeExpression(seenExp)) ||
                        findmultiply(serializedExp, serializeExpression(seenExp)))) {
                        isDuplicate = true;
                        break;
                    }
                }
            }
            if (!isDuplicate && ans == 24) {
                flag = true;
                seenExpressions.insert(serializedExp);
                operation += exp;
            }
            return;
        }

        for (int i = 0; i < 4; ++i) {
            if (!vis[i]) {
                vis[i] = true;
                dfs(ans + start[i], x + 1, format(exp, '+', std::to_string(start[i])));
                dfs(ans - start[i], x + 1, format(exp, '-', std::to_string(start[i])));
                dfs(ans * start[i], x + 1, format(exp, '*', std::to_string(start[i])));
                if (start[i] != 0) {
                    dfs(ans / start[i], x + 1, format(exp, '/', std::to_string(start[i])));
                }
                vis[i] = false;
            }
        }
    };

    for (int i = 0; i < 4; i++) {
        vis[i] = true;
        dfs(start[i], 1, std::to_string(start[i]));
        vis[i] = false;
    }

    if (flag) {
        std::cout << "yes" << '\n';
        std::cout << operation << '\n';
    } else {
        std::cout << "no" << '\n';
    }

    return 0;
}