#include <iostream>
#include <functional>
#include <string>
#include <set>
#include <cctype>
#include <sstream>

std::string serializeExpression(const std::string& exp) {
    std::string result;
    for (char c : exp) {
        bool inParentheses = false;
        if (std::isspace(c)) {
            inParentheses = true;
        }
        if (!inParentheses) {
            result += c;
        }
    }
    return result;
}

std::function<bool(const std::string&, const std::string&)> findmultiply = [] (const std::string& str1, const std::string& str2) -> bool {
    size_t pos1 = str1.find('*');
    size_t pos2 = str2.find('*');
    if(str1[pos1 - 1] == ')' && str1[pos1 + 1] == '(') return false;
    if(str2[pos2 - 1] == ')' && str2[pos2 + 1] == '(') return false;

    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        return false;
    }

    int a1 = str1[pos1 - 1];
    int a2 = str1[pos1 + 1];
    int b1 = str2[pos2 - 1];
    int b2 = str2[pos2 + 1];

    return (a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1);
};

std::function<bool(const std::string&, const std::string&)> findplus = [] (const std::string& str1, const std::string& str2) -> bool {
    size_t pos1 = str1.find('+');
    size_t pos2 = str2.find('+');
    if(str1[pos1 - 1] == ')'&& str1[pos1 + 1] == '(') return false;
    if(str2[pos2 - 1] == ')'&& str2[pos2 + 1] == '(') return false;

    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        return false;
    }

    int a1 = str1[pos1 - 1];
    int a2 = str1[pos1 + 1];
    int b1 = str2[pos2 - 1];
    int b2 = str2[pos2 + 1];

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
                return ;
            }
            return ;
        }
        if(x == 2) {
            for(int i = 0; i < 4 ;i++) {
                for(int j = i + 1; j < 4; j++) {
                    if(i != j && !vis[i] && !vis[j]) {
                        std::vector<std::pair<double, std::string>> results = {
                        {start[i] + start[j], "(" + std::to_string(start[i]) + " + " + std::to_string(start[j]) + ")"},
                        {start[i] - start[j], "(" + std::to_string(start[i]) + " - " + std::to_string(start[j]) + ")"},
                        {start[j] - start[i], "(" + std::to_string(start[j]) + " - " + std::to_string(start[i]) + ")"},
                        {start[i] * start[j], "(" + std::to_string(start[i]) + " * " + std::to_string(start[j]) + ")"},
                        };
                        if (start[j] != 0) {
                            results.push_back({start[i] / start[j], "(" + std::to_string(start[i]) + " / " + std::to_string(start[j]) + ")"});
                        }
                        if (start[i] != 0) {
                            results.push_back({start[j] / start[i], "(" + std::to_string(start[j]) + " / " + std::to_string(start[i]) + ")"});
                        }
                        for(const auto& result : results) {
                            dfs(ans + result.first,x + 2,format(exp, '+', result.second));
                            dfs(ans - result.first,x + 2,format(exp, '-', result.second));
                            dfs(ans * result.first,x + 2,format(exp, '*', result.second));
                            dfs(ans / result.first,x + 2,format(exp, '/', result.second));
                        }
                    }

                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            if (!vis[i]) {
                vis[i] = true;
                dfs(ans + start[i], x + 1, format(exp, '+', std::to_string(start[i])));
                dfs(ans - start[i], x + 1, format(exp, '-', std::to_string(start[i])));
                dfs(ans * start[i], x + 1, format(exp, '*', std::to_string(start[i])));
                dfs(ans / start[i], x + 1, format(exp, '/', std::to_string(start[i])));
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