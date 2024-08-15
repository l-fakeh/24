#include<iostream>
#include<functional>
#include<string>
int start[4];
bool vis[4];
std::string operation;
int main() {
    for (auto& x : start) {
        std::cin >> x;
    }
    std::function<std::string(const std::string,char,const std::string)> format = [&](const std::string& op1,char op, const std::string& op2) -> std::string{
        return "(" + op1 + " " + op + " " + op2 + ")";
    };
    bool flag = false;
    std::function<void(double, int,const std::string)> dfs = [&](double ans, int x,const std::string& exp) {
        if (x == 4) {
            if (ans == 24) {
                flag = true;
                operation += exp;
            }
            return;
        }
        
        for (int i = 0; i < 4; ++i) {
            if (!vis[i]) {
                vis[i] = true;
                dfs(ans + start[i], x + 1, format(exp,'+',std::to_string(start[i])));
                dfs(ans - start[i], x + 1, format(exp,'-',std::to_string(start[i])));
                dfs(ans * start[i], x + 1, format(exp,'*',std::to_string(start[i])));
                if (start[i] != 0) {  
                    dfs(ans / start[i], x + 1, format(exp,'/',std::to_string(start[i])));
                }
                vis[i] = false;
            }
        }
    };
    for(int i = 0;i < 4; i++) {
        vis[i] = true;
        dfs(start[i],1,std::to_string(start[i]));
        vis[i] = false;
    }
    if(flag) std::cout << "yes" << '\n';
    else std::cout << "no" << '\n';
    std::cout << operation << '\n';
    return 0;
}