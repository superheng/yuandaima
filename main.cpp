/*
* Should probably re-write this from scratch
* Needs optimization(implementation of scatter reads, just better/more optimized code, etc..) and better fail-safes, logging etc.
* More readable/cleaner code by for example creating a console header for output to reduce lines written for setting color
* etc..
*
* Works as is, but lots to do if you want the "perfect" code.
*
* I tried commenting it as best I could, if I missed something. Soz.
* I don't recommend actually using this as a base and instead trying to improve it or re-write it from scratch.
*/

#pragma warning(disable:4200) // ignore vmm header and lc header throwing warnings

#include <Windows.h>                    // include Windows API header                               [system(), SetConsoleTextAttribute(), ...]
#include <cstdio>                       // include standard input/output functions                  [printf()]
#include <conio.h>                      // include console input/output functions                   [_getch()]
#include <thread>                       // include thread library for multithreading                [std::thread, std::this_thread::sleep_for()]
#include <fstream>                      // include file stream library for file input/output        [std::ifstream, std::ofstream]
#include <sstream>                      // include string stream library for string manipulation    [std::stringstream]
#include <iomanip>                      // include manipulators library for formatted output        [std::setw(), std::setfill()]
#include <unordered_map>                // include unordered map class for fast key-value pairs     [std::unordered_map]
#include "offsets.hpp"
#include <string>
#include "aimbot.hpp"
#include "IMGUI/glfw3.h"
#include"IMGUI/imgui.h"
#include "cf.hpp"
#include"Cprotect.h"
#define SEP  "\t"
#define GET_TIME std::to_string(std::time(0))
using namespace std;
int 获取 = 0;
//struct curl_slist* cookies = NULL;//返回的cookies
//long httpcode;//网页状态码
//FILE* fp;//QR文件指针

std::string getResponseStr;
std::string qrsig;
std::string skey;
std::string sigx;
std::string uin;
std::string pt4_token;
std::string p_skey;
std::string bkn;
std::string ptqrtoken;

#define GET_PNG		0
#define GET_JSON	1

char* getCurrentTime(int out);
void set_qrsig();

//get请求和post请求数据响应函数
//size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
//{
//    //在注释的里面可以打印请求流，cookie的信息
//    //cout << "----->reply" << endl;
//    string* str = (string*)stream;
//    //cout << *str << endl;
//    (*str).append((char*)ptr, size * nmemb);
//    return size * nmemb;
//}
////写入文件函数
//size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
//{
//    int written = fwrite(ptr, size, nmemb, (FILE*)fp);
//    return written;
//}
//void PrintCK() {
//    while (cookies) {
//        cout << cookies->data << endl;
//        cookies = cookies->next;
//    }
//}
//
////http GET请求  
//CURLcode curl_get_req(const std::string& url, std::string& response, int type)
//{
//    //curl初始化  
//    CURL* curl = curl_easy_init();
//    // curl返回值 
//    CURLcode res;
//    if (curl)
//    {
//        //设置curl的请求头
//        struct curl_slist* header_list = NULL;
//        std::string cookieFile = "C:\\cookie.txt";
//        //header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
//        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36");
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
//
//        //不接收响应头数据0代表不接收 1代表接收
//        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
//
//        //设置请求的URL地址 
//        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//
//        //设置ssl验证
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
//
//        //文件形式输出cookie
//        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");//启动cookie引擎
//
//        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
//
//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
//
//        //设置数据接收函数
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
//        switch (type) {
//        case GET_PNG:
//            fp = fopen(response.c_str(), "wb");
//            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//            break;
//        case GET_JSON:
//            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response); break;
//        }
//
//        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
//
//        //设置超时时间
//        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time  
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);
//
//        // 开启请求  
//        res = curl_easy_perform(curl);
//        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);//保存cookie到字符串cookies中
//        //printf("Cookies:%s\n", cookies->data);//原Cookie
//    }
//    // 释放curl 
//    curl_easy_cleanup(curl);
//
//    return res;
//}
//CURLcode curl_get_req(const std::string& url, std::string& response, int type, std::string cookie, int head = 0)
//{
//    //curl初始化  
//    CURL* curl = curl_easy_init();
//    // curl返回值 
//    CURLcode res;
//    if (curl)
//    {
//        //设置curl的请求头
//        struct curl_slist* header_list = NULL;
//        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36");
//        std::string ck = "Cookie:" + cookie;
//        header_list = curl_slist_append(header_list, ck.c_str());//请求头中的cookie
//        cookies = NULL;
//
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
//
//        //接收响应头数据0代表不接收 1代表接收
//        curl_easy_setopt(curl, CURLOPT_HEADER, head);
//
//        //设置请求的URL地址 
//        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//
//        //获取网页请求状态码
//        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpcode);
//
//        //允许几次重定向
//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
//
//        //设置ssl验证
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
//
//        //设置cookie
//        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
//
//        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
//
//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
//
//        //设置数据接收函数
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
//        switch (type) {
//        case GET_PNG:
//            fp = fopen(response.c_str(), "wb");
//            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//            break;
//        case GET_JSON:
//            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response); break;
//        }
//
//        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 0);
//
//        //设置超时时间
//        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6); // set transport and time out time  
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);
//
//        // 开启请求  
//        res = curl_easy_perform(curl);
//        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);//保存cookie到字符串cookies中
//    }
//    // 释放curl 
//    curl_easy_cleanup(curl);
//    return res;
//}
//
////http POST请求  
//CURLcode curl_post_req(const string& url, const string& postParams, string& response, std::string cookie)
//{
//    // curl初始化  
//    CURL* curl = curl_easy_init();
//    // curl返回值 
//    CURLcode res;
//    if (curl)
//    {
//        // set params
//        //设置curl的请求头
//        struct curl_slist* header_list = NULL;
//        header_list = curl_slist_append(header_list, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko");
//        header_list = curl_slist_append(header_list, "Content-Type:application/x-www-form-urlencoded; charset=UTF-8");
//        std::string ck = "Cookie:" + cookie;
//        header_list = curl_slist_append(header_list, ck.c_str());
//        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
//
//        //不接收响应头数据0代表不接收 1代表接收
//        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
//
//        //设置请求为post请求
//        curl_easy_setopt(curl, CURLOPT_POST, 1);
//
//        //设置请求的URL地址
//        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//        //设置post请求的参数
//        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str());
//
//        //设置ssl验证
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
//        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
//
//        //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
//
//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
//
//        //设置数据接收和写入函数
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
//
//        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
//
//        //设置超时时间
//        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6);
//        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 6);
//
//        // 开启post请求
//        res = curl_easy_perform(curl);
//    }
//    //释放curl 
//    curl_easy_cleanup(curl);
//    return res;
//}
//
////bkn的算法
//int set_bkn() {
//    int t = 5381, n = 0, o = skey.length();
//    while (n < o)
//    {
//        t += (t << 5) + toascii(skey.at(n));
//        n++;
//    }
//    return (t & 2147483647);
//}
////获取qrsig
//void set_qrsig() {
//    char qr[] = "qrsig";
//    char* result;
//    char* cookie = cookies->data;
//    result = strstr(cookie, qr);
//    result += sizeof("qrsig");
//    qrsig = result;
//    //printf("%s\n", result);
//}
//
////获取ptqrtoken
//int set_ptqrtoken() {
//    //qrsig = "enR8pbm6qeWQIu7C33CT*wpwg2upFruEL4lB6CB3wfaSacRtvHtAS2JbamaA1HdW";
//    int len = qrsig.length(), i = 0, e = 0;
//    while (len > i) {
//        e += (e << 5) + toascii(qrsig.at(i));
//        i++;
//    }
//    return (2147483647 & e);
//}
//char* getSigx(std::string str) {
//    char sigxSign[] = "&ptsigx=", * result{};
//    char* url = (char*)str.c_str(), * point;
//    point = strstr(url, sigxSign);
//    point += sizeof("&ptsigx");
//    std::string a = point;
//    for (int i = 0; i < 145; i++)
//    {
//        sigx += a.at(i);
//    }
//    return result;
//}
//
////获取登陆后的url
//char* getSkeyUrl(std::string str) {
//    /*char* url = "https://ptlogin2.qun.qq.com/";
//    char* skey = "skey";
//    char* res = strstr((char*)str.c_str(), skey);
//    cout << res;*/
//    int i = 0;
//    char* res = strtok((char*)str.c_str(), "','"); i++;
//    while (res != NULL && i != 4) {
//        res = strtok(NULL, "','"); i++;
//    }
//    return res;
//}
//std::string getSkey() {
//    int a = 0;
//    int pt4_tokenBool = 0;
//    while (cookies && a <= 4) {
//        int i = 0;
//        const char* res = strtok(cookies->data, "\t");
//        i++;
//        while (res != NULL && i != 6) {
//            res = strtok(NULL, "\t"); i++;
//        }
//        if (stricmp(res, "skey") == 0) {
//            skey = res = strtok(NULL, "\t");
//            a++;
//        }
//        else if (stricmp(res, "p_uin") == 0 || stricmp(res, "uin") == 0) {
//            res = strtok(NULL, "\t");
//            uin = res;
//            a++;
//        }
//        else if (stricmp(res, "pt4_token") == 0 && pt4_tokenBool == 0)
//        {
//            res = strtok(NULL, "\t");
//            pt4_token = res;
//            pt4_tokenBool = 1;
//            a++;
//        }
//        else if (stricmp(res, "p_skey") == 0)
//        {
//            res = strtok(NULL, "\t");
//            p_skey = res; a++;
//        }
//        else
//        {
//            cookies = cookies->next;
//            continue;
//        }
//        cookies = cookies->next;
//
//    }
//    return skey;
//}
//
////判断curl返回结果是否正确
//bool curlRes(CURLcode res) {
//    if (res == CURLE_OK)
//    {
//        return true;
//    }
//    else {
//        printf("Curl perform failed: %s\n", curl_easy_strerror(res));
//        return false;
//    }
//}
////发送心跳包获取当前状态
//void heartPack() {
//    while (1) {
//        std::string url = "https://ssl.ptlogin2.qq.com/ptqrlogin?u1=https%3A%2F%2Fqun.qq.com%2Fmanage.html%23click&ptqrtoken=" +
//            ptqrtoken + "&ptredirect=1&h=1&t=1&g=1&from_ui=1&ptlang=2052&action=0-0-" +
//            GET_TIME + "&js_ver=20032614&js_type=1&login_sig=&pt_uistyle=40&aid=715030901&daid=73&";
//        std::string getResponseStr;
//        std::string my_cookie = "qrsig=" + qrsig;
//        auto res = curl_get_req(url, getResponseStr, GET_JSON, my_cookie);
//        getResponseStr = Utf8ToGb2312(getResponseStr.c_str());
//        if (curlRes(res)) {
//            if (strstr(getResponseStr.c_str(), "二维码未失效")) {//strstr
//                printf("二维码未失效，%s\r", getCurrentTime(0));
//            }
//            else if (strstr(getResponseStr.c_str(), "二维码认证中"))
//            {
//                printf("二维码认证中，%s\r", getCurrentTime(0));
//            }
//            else if (strstr(getResponseStr.c_str(), "二维码已失效"))
//            {
//                printf("二维码已失效，%s\r", getCurrentTime(0));
//            }
//            else {               
//               // PrintCK();
//                printf("登录成功，%s\n", getCurrentTime(0));
//                cout << cookies->data << endl;
//                getSkey();          
//                std::string loginUrl = getSkeyUrl(getResponseStr);
//                getSigx(loginUrl);
//                loginUrl = "https://ptlogin2.qun.qq.com/check_sig?pttype=1&uin=" + uin + "&service=ptqrlogin&nodirect=0&ptsigx=" +
//                    sigx + "&s_url=https%3A%2F%2Fqun.qq.com%2Fmanage.html&f_url=&ptlang=2052&ptredirect=101&aid=715030901&daid=73&j_later=0&low_login_hour=0&regmaster=0&pt_login_type=3&pt_aid=0&pt_aaid=16&pt_light=0&pt_3rd_aid=0";
//                res = curl_get_req(loginUrl, getResponseStr, GET_JSON, my_cookie, 0);
//                if (curlRes(res)) {
//                    skey = getSkey();
//                    bkn = to_string(set_bkn());//算出bkn
//                }
//                
//                break;
//            }
//        }
//        else
//        {
//
//            return;
//        }
//    }
//}
//
//bool findQun(std::string str) {
//    const char* a = strstr(str.c_str(), "\"gc\":642524202");//使用"gc":qq群号的串，避免因某个群主的qq号与目标群号相同而导致验证失误（虽然极小概率就是了）
//    if (a == NULL) {
//        return false;
//    }
//    else return true;
//}
//
//bool getQun() {
//    std::string url = "https://qun.qq.com/cgi-bin/qun_mgr/get_group_list";
//    string resPost0;
//    std::string bknS = "bkn=" + bkn;
//    std::string post_cookie = "skey=" + skey + ";uin=" + uin + ";p_skey=" + p_skey + ";p_uin=" + uin + ";pt4_token = " + pt4_token;
//    auto res = curl_post_req(url, bknS, resPost0, post_cookie);
//    if (curlRes(res)) {
//        //cout << resPost0 << endl;
//        return findQun(resPost0);
//    }
//}
//


//获取当前时间并输出
char* getCurrentTime(int out = 1) {
    time_t rawtime;
    struct tm* info;
    char buffer[80];

    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);//时间格式 YYYY-MM-DD HH:MM:SS
    if (out) {
        printf("%s", buffer);
    }
    return buffer;
}


// define constants for arrow keys and enter key
#define MAX_JSON_SIZE 4096
#define UP_ARROW   72
#define DOWN_ARROW 80
#define ENTER      13
int     sightx = globals::ScreenWidth / 2;
int     sighty = globals::ScreenHeight / 2;



bool setting = true;
bool SETUP_Handler();
void EXIT_Handler();
void ClearScreen();
bool GET_Process();
void GET_InGame();
//void aimbot();
//新的
void thread_getList();
void thread_getListInfo();
void triggerbot();
void other();
void 鬼跳();
void 碎步();
void 红名();
void USP();
char ip[] = { 49,53,52,46,52,48,46,53,53,46,49,57,50,0 };
char ip2[] = { 49,53,52,46,52,48,46,53,53,46,49,57,50,0 };
typedef struct StringA
{
    char buffer[64];
};
string GetConfigurationItem(string FileName, string SectionName, string ItemName, string DefaultValue)
{
    CHAR Buf[1024];
    GetPrivateProfileStringA(SectionName.c_str(), ItemName.c_str(), DefaultValue.c_str(), Buf, 1024, FileName.c_str());
    return Buf;
}

string 取当前武器名(int 武器Index)
{
    
    LONG64 武器地址 = read<ULONG64>(read<ULONG64>(globals::qProcessBase + offset_wuqidizhi) + (武器Index * 8));
    auto realname = read<StringA>(武器地址 + 14);
   
    return realname.buffer;
}
/*
LONG WINAPI GlobalExceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
    // 获取错误代码和异常地址
    DWORD errorCode = exceptionInfo->ExceptionRecord->ExceptionCode;
    PVOID exceptionAddress = exceptionInfo->ExceptionRecord->ExceptionAddress;

    // 构建提示框消息
    std::string message = "Exception occurred!\n";
    message += "Error code: 0x" + std::to_string(errorCode) + "\n";
    message += "Exception address: 0x" + std::to_string(reinterpret_cast<uintptr_t>(exceptionAddress));

    // 使用 MessageBox 弹出提示框
    MessageBoxA(NULL, message.c_str(), "Exception", MB_ICONERROR | MB_OK);

    // 返回 EXCEPTION_EXECUTE_HANDLER 表示异常已被处理
    return EXCEPTION_EXECUTE_HANDLER;
}
*/
int main()
{
    //_SetMachinecode(TRUE, TRUE, FALSE, FALSE, FALSE);
    //_Verificationbegins(ip, HOSTSHORT, COPYRIGHT, VERSIONNUMBER, NULL);
    //char buf[1000] = { 0 };

  /*  string Card = GetConfigurationItem("C:\\User.ini", "Configure", "Key", "NO");
    const char* p = Card.data();
    cout << "卡密" << p << endl;*/
    //Sleep(1000);
    //int result1 = std::strcmp(ip, ip2);
    ////printf("result1%d\m", result1);
    //if (result1 != 0)
    //{

    //	exit(0);

    //}

    //_CardUntie(p, buf);
    //if (_CardLogin(p, buf))
    //{


    //    cout << "验证成功进入" << endl;

    //    获取 = 1;
        //_CshieldVM();

    //}
    //else
    //{
    //    cout << "失败,请检查卡密是否无误" << buf << endl;
    //    Sleep(2000);
    //    exit(0);
    //}

    //SetUnhandledExceptionFilter(GlobalExceptionHandler);
    // create a new thread to handle setup

            ShowWindow(GetConsoleWindow(), SW_HIDE);//隐藏黑框
    
    //std::string QRPath = "QR.png";//QR文件位置
    ////system("chcp 65001");
    //system("cls");
    //string getUrlStr = "https://ssl.ptlogin2.qq.com/ptqrshow?appid=715030901&e=2&l=M&s=3&d=72&v=4&t=0." + std::to_string(std::time(0)) + "&daid=73&pt_3rd_aid=0";
    ////printf("%s\n", getUrlStr);
    //auto res = curl_get_req(getUrlStr, QRPath, GET_PNG);//获取二维码
    ////if (curlRes(res))
    ////{
    ////    //cout << getResponseStr << endl;
    ////    fclose(fp);
    ////    printf("登录二维码获取成功，请打开同目录下的QR.png扫码进行验证"); getCurrentTime();//输出相应提示
    ////    printf("\n");
    ////    set_qrsig();
    ////    ptqrtoken = to_string(set_ptqrtoken());
    ////    heartPack();
    ////    if (getQun())
    ////    {
    ////        cout << "验证成功" << endl;
    ////        
    ////    }
    ////    else
    ////    {
    ////        cout << "验证失败,你没有加入qq群:642524202" << endl;

    //        cout << "End" << endl;
    //        return 0;

    //    }
    //}

    //if (获取!=1)
    //{
    //    exit(0);
    //}
            std::thread statusThread(GET_InGame);                 // thread for checking if user is in-game
            std::thread ListThread(thread_getList);      
            std::thread otherThread(other); //视角分散
            std::thread bugThread(鬼跳);
            std::thread flyThread(碎步);
            std::thread hmhread(红名);
            //std::thread usphread(USP);
            /*std::thread flyThread(飞天);*/


            std::string configFilePath = "CFconfig.ini";

            // 读取配置文件
            LoadConfig(configFilePath);

            // 保存配置文件
            SaveConfig(configFilePath);


            std::thread setupThread(SETUP_Handler);
            setupThread.join(); // <-- main() has to wait for setup to complete before proceeding

            std::thread processThread(GET_Process);


                    // Create application window
                   //ImGui_ImplWin32_EnableDpiAwareness();
                    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("CROSSFIRE"), NULL };
                    ::RegisterClassExW(&wc);
                    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("CROSSFIRE"), WS_POPUP, 0, 0, globals::ScreenWidth, globals::ScreenHeight, NULL, NULL, wc.hInstance, NULL);

                    ImGui_ImplWin32_EnableAlphaCompositing(hwnd);

                    if (!CreateDeviceD3D(hwnd))
                    {
                        CleanupDeviceD3D();
                        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
                        return 1;
                    }


                    ::ShowWindow(hwnd, 10);
                    ::UpdateWindow(hwnd);


                    ImGui_ImplWin32_EnableAlphaCompositing(hwnd);//窗口透明

                    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
                    exStyle |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
                    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);//鼠标穿透


                    IMGUI_CHECKVERSION();
                    ImGui::CreateContext();
                    ImGuiIO& io = ImGui::GetIO(); (void)io;
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
                    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

                    ImGui::StyleColorsLight();



                    // Setup Platform/Renderer backends
                    ImGui_ImplWin32_Init(hwnd);
                    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
                    ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
                    /*ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Users\\Administrator\\Appdata\\Local\\Microsoft\\Windows\\Fonts\\AAYUANWEITUSI-2.TTF", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());*/
                    
                    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

                    // Main loop
                    bool done = false;
                    
                    
                    //自瞄热键
                    int aimselect = 0; // 初始选中第一个选项
                    const char* items[] = { u8"左键", u8"右键", u8"左右键" };
                    const int itemCount = sizeof(items) / sizeof(items[0]);
                    int defaultIndex = globals::aimbotkey - 1;  // 根据 aimbotkey 计算默认选项的索引
                    //自瞄部位
                    int boneselect = 1; // 初始选中第一个选项
                    const char* boneitems[] = { u8"头", u8"胸", u8"胃" };
                    const int boneitemCount = sizeof(boneitems) / sizeof(boneitems[0]);
                    int boneIndex = aim::aimbone;  // 根据 aimbotkey 计算默认选项的索引



                    static bool WinPos = true;//用于初始化窗口位置
                    while (!done)
                    {
                       
                        // Poll and handle messages (inputs, window resize, etc.)
                        // See the WndProc() function below for our to dispatch events to the Win32 backend.
                        MSG msg;
                        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
                        {
                            ::TranslateMessage(&msg);
                            ::DispatchMessage(&msg);
                            if (msg.message == WM_QUIT)
                                done = true;
                        }
                        if (done)
                            break;

                        // Start the Dear ImGui frame
                        ImGui_ImplDX11_NewFrame();
                        ImGui_ImplWin32_NewFrame();
                        ImGui::NewFrame();
                        
                        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//设置优先级别最高
                        update_key_state_bitmap();
                       
                        if (is_key_down(36))//insert
                        {
                            setting = !setting;  // 切换ImGui显示状态
                            Sleep(200);
                        }

                        thread_getListInfo();


                        if (setting == true)
                        {
                            if (globals::autokmnet && globals::kmnet == false)
                            {
                                kmNet_init(aim::netip, aim::netport, aim::netuuid);
                                kmNet_monitor(1);
                                globals::kmnet = true;
                            }
                            if (globals::autokm && globals::km == false)
                            {
                                string port = find_port("USB-SERIAL CH340"); // name of the kmbox port without ( COM )
                                if (port.empty()) {
                                    std::cout << "\n	[!] no port found..";



                                }
                                if (!open_port(hSerial, port.c_str(), globals::波特率)) { // CBR_1115200 is the baud rate
                                    std::cout << "\n	[!] opening the port failed!\n";



                                }
                                else
                                {
                                    std::cout << "\n	[+] connected to the kmbox with " + port;

                                    globals::km = true;
                                }

                            }
                            if (WinPos)//初始化窗口
                            {
                                ImGui::SetNextWindowPos({ float(globals::ScreenWidth - 600) / 2,float(globals::ScreenHeight - 400) / 2 });
                                ImGui::SetWindowSize({ 600.0f,400.0f });//设置窗口大小
                                WinPos = false;//初始化完毕
                            }

                            ImGuiWindowClass noAutoMerge;
                            noAutoMerge.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoAutoMerge;
                            ImGui::SetNextWindowClass(&noAutoMerge);//自动脱离

                            ImGui::Begin(u8" RS (主机 Home 显示/隐藏)", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);//开始绘制窗口
                            ImGui::Checkbox(u8"自动分辨率", &globals::自动分辨率);
                            if (globals::自动分辨率)
                            {
                                globals::ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
                                globals::ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
                            }
                            else
                            {
                                char 宽[32];
                                snprintf(宽, 32, "%d", globals::ScreenWidth);
                                ImGui::SetNextItemWidth(150);
                                ImGui::SameLine();
                                if (ImGui::InputText(u8"宽", 宽, 32, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                                    globals::ScreenWidth = atoi(宽);
                                }
                                char 高[32];
                                snprintf(高, 32, "%d", globals::ScreenHeight);
                                ImGui::SetNextItemWidth(150);
                                ImGui::SameLine();
                                if (ImGui::InputText(u8"高", 高, 32, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                                    globals::ScreenHeight = atoi(高);
                                }
                            }
                            char dist[64];
                            sprintf_s(dist, "(%.1f FPS)\n", ImGui::GetIO().Framerate);
                            ImGui::Text(dist);
                            ImGui::Text(u8"FPGA状态:");
                            ImGui::SameLine();
                            if (globals::INIT_SUCCESS == true)
                            {
                                ImGui::TextColored(ImVec4(0, 238.0f, 0.0f, 1.0f), u8"已连接");
                            }
                            else
                            {
                                ImGui::TextColored(ImVec4(255.f, 0.f, 0.0f, 1.0f), u8"未连接");
                            }
                            ImGui::SameLine();
                            ImGui::Text(u8"进程状态:");
                            ImGui::SameLine();
                            if (globals::ProcessStatus == true)
                            {
                                ImGui::TextColored(ImVec4(0, 238.0f, 0.0f, 1.0f), u8"已获取进程");
                            }
                            else
                            {
                                ImGui::TextColored(ImVec4(255.f, 0.f, 0.0f, 1.0f), u8"未获取进程");
                            }
                            //ImGui::Checkbox(u8"绘制背景(融合器/串流)", &aim::融合器);
                            ImGui::Text(u8"kmboxnet状态:");
                            ImGui::SameLine();
                            if (globals::kmnet == true)
                            {
                                ImGui::TextColored(ImVec4(0, 238.0f, 0.0f, 1.0f), u8"已连接");
                            }
                            else
                            {
                                ImGui::TextColored(ImVec4(255.f, 0.f, 0.0f, 1.0f), u8"未连接");
                            }
                            ImGui::SameLine();
                            ImGui::Text(u8"kmboxb+状态:");
                            ImGui::SameLine();
                            if (globals::km == true)
                            {
                                ImGui::TextColored(ImVec4(0, 238.0f, 0.0f, 1.0f), u8"已连接");
                            }
                            else
                            {
                                ImGui::TextColored(ImVec4(255.f, 0.f, 0.0f, 1.0f), u8"未连接");
                            }
                            if (aim::融合器)
                            {
                                clear_color = ImVec4(0.0f, 0.0f, 0.0f, 255.00f);
                            }
                            if (aim::融合器 == false)
                            {
                                clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
                            }
                            ImGui::NextColumn();
                            ImGui::Text(u8"当前对局状态:");
                            ImGui::SameLine();
                            if (globals::IN_GAME == true)
                            {
                                ImGui::TextColored(ImVec4(0, 238.0f, 0.0f, 1.0f), u8"已进入对局");
                            }
                            else
                            {
                                ImGui::Text(u8"大厅等待中");
                            }
                            ImGui::Text(u8"实体数量:%zd", globals::entityInfoVector.size());
                            
                            ImGui::BeginTabBar(u8"设置");
                            // esp menu
                            if (ImGui::BeginTabItem(u8"透视"))
                            {
                                ImGui::Checkbox(u8"绘制背景(融合器/串流)", &aim::融合器);
                                ImGui::Checkbox(u8"方框透视", &aim::boxesp);
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"骨骼透视", &aim::skeletonesp);  
                                
                                ImGui::Checkbox(u8"预瞄显示", &aim::显示头部);
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"显示准星", &aim::crosshair);
                                
                                ImGui::Checkbox(u8"显示名字", &aim::显示名字);
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"显示血条", &aim::hp);
                                ImGui::ColorEdit4(u8"方框颜色", (float*)&globals::rectangleColor, ImGuiColorEditFlags_NoInputs);                              
                                ImGui::EndTabItem();                            
                            }
                            if (ImGui::BeginTabItem(u8"自瞄"))
                            {
                                /*ImGui::Checkbox(u8"自动扳机", &aim::triggerbot);
                                ImGui::SameLine();*/                          
                                ImGui::Checkbox(u8"自动瞄准", &aim::aimbot);
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"显示范围", &aim::范围判断);
                                //ImGui::SameLine();
                                ImGui::Checkbox(u8"Shift锁头", &aim::锁头判断);
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"红名吸附(等待优化)", &aim::红名吸附);
                                if (aim::aimbot == true)
                                {
                                    aim::红名吸附 = false;
                                }
                                if (aim::红名吸附 == true)
                                {
                                    aim::aimbot = false;
                                }
                                /*ImGui::SetNextItemWidth(200);
                                ImGui::InputInt(u8"扳机延时", &aim::triggerbotkey);*/
                                ImGui::SetNextItemWidth(200);
                                ImGui::InputInt(u8"自瞄热键", &aim::aimkey);
                               /* ImGui::SetNextItemWidth(200);
                                ImGui::InputInt(u8"瞄准时间（仅Net）", &aim::aimkey);*/
                                ImGui::SetNextItemWidth(200);
                                ImGui::InputInt(u8"自瞄部位(4胸5脖6头)", &aim::aimbone);
                                ImGui::SetNextItemWidth(200);
                                ImGui::SliderInt(u8"自瞄FOV", &aim::fov, 1, 300);
                                ImGui::SetNextItemWidth(150);
                                ImGui::SliderFloat(u8"X轴速率", &aim::factorx, 1, 10, "%.0f");
                                ImGui::SetNextItemWidth(150);
                                ImGui::SliderFloat(u8"Y轴速率", &aim::factory, 1, 10, "%.0f");
                                ImGui::EndTabItem();
                            }                       
                            if (ImGui::BeginTabItem(u8"kmbox"))
                            {
                                ImGui::Text(u8"kmboxnet参数区:");
                                ImGui::SetNextItemWidth(150);
                                ImGui::InputText("ip", aim::netip, sizeof(aim::netip));
                                ImGui::SetNextItemWidth(150);
                                ImGui::InputText("port", aim::netport, sizeof(aim::netport));
                                ImGui::SetNextItemWidth(150);
                                ImGui::InputText("uuid", aim::netuuid, sizeof(aim::netuuid));
                                ImGui::SameLine();
                                if (ImGui::Button(u8"打开kmnet") && globals::kmnet == false)
                                {

                                    globals::kmnet = true;
                                    kmNet_init(aim::netip, aim::netport, aim::netuuid);
                                    kmNet_monitor(1);



                                }
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"自动打开net", &globals::autokmnet);
                               

                                ImGui::Text(u8"kmboxbpro参数区:");
                                char buffer[32];
                                snprintf(buffer, 32, "%d", globals::波特率);
                                ImGui::SetNextItemWidth(150);
                                if (ImGui::InputText(u8"波特率", buffer, 32, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                                    globals::波特率 = atoi(buffer);
                                }
                                ImGui::SameLine();
                                //  ImGui::Checkbox(u8"kmboxb+", &globals::kmboxbpro);
                                if (ImGui::Button(u8"打开b+") && globals::km == false)
                                {
                                    string port = find_port("USB-SERIAL CH340"); // name of the kmbox port without ( COM )
                                    if (port.empty()) {
                                        std::cout << "\n	[!] no port found..";



                                    }
                                    if (!open_port(hSerial, port.c_str(), globals::波特率)) { // CBR_1115200 is the baud rate
                                        std::cout << "\n	[!] opening the port failed!\n";



                                    }
                                    else
                                    {
                                        std::cout << "\n	[+] connected to the kmbox with " + port;

                                        globals::km = true;
                                    }
                                }
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"自动打开B+", &globals::autokm);
                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem(u8"其他功能"))
                            {
                                ImGui::Checkbox(u8"红名扳机（狙）", &aim::红名扳机);
                                ImGui::SameLine();
                                ImGui::Checkbox(u8"自动碎步", &aim::碎步判断);
                                //ImGui::SameLine();
                                ImGui::Checkbox(u8"自动鬼跳", &aim::鬼跳判断);
                                /*ImGui::SameLine();
                                ImGui::Checkbox(u8"自动USP", &aim::USP判断);*/
                                ImGui::SetNextItemWidth(200);
                                ImGui::InputInt(u8"碎步热键", &aim::suikey);
                                ImGui::SetNextItemWidth(200);
                                ImGui::InputInt(u8"鬼跳热键", &aim::guikey);
                                ImGui::SetNextItemWidth(200);
                                /*ImGui::InputInt(u8"USP热键", &aim::USPkey);
                                ImGui::SetNextItemWidth(200);*/
                                //ImGui::InputInt(u8"扳机延时", &aim::bantime);
                                ImGui::SliderFloat(u8"扳机延时", &aim::bantime, 5, 50, "%.0f");
                                ImGui::SetNextItemWidth(200);
                                //ImGui::InputInt(u8"碎步延时", &aim::suitime);
                                ImGui::SliderFloat(u8"碎步延时", &aim::suitime, 10, 50, "%.0f");
                                ImGui::SetNextItemWidth(200);
                                //ImGui::InputInt(u8"鬼跳延时", &aim::guitime);
                                ImGui::SliderFloat(u8"鬼跳延时", &aim::guitime, 10, 50, "%.0f");
                                /*ImGui::SetNextItemWidth(150);
                                ImGui::SliderFloat(u8"USP延时", &aim::USPtime, 50, 100, "%.0f");*/
                                ImGui::EndTabItem();
                            }
                            if (ImGui::Button(u8"退出程序"))
                            {
                                VMMDLL_Close(globals::hVMM);
                                exit(0);
                            }
                            ImGui::SameLine(200);
                            if (ImGui::Button(u8"保存配置"))
                            {
                                globals::inireload = true;
                                ReloadConfig();
                            }

                            ImGui::EndTabBar();
                            ImGui::End();

                        }







                        // Rendering
                        ImGui::Render();
                        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
                        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
                        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
                        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

                        // Update and Render additional Platform Windows
                        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                        {
                            ImGui::UpdatePlatformWindows();
                            ImGui::RenderPlatformWindowsDefault();
                        }
                      
                        g_pSwapChain->Present(0, 0); // Present with vsync
                        //g_pSwapChain->Present(0, 0); // Present without vsync
                    }



                    // Cleanup
                    ImGui_ImplDX11_Shutdown();
                    ImGui_ImplWin32_Shutdown();
                    ImGui::DestroyContext();

                    CleanupDeviceD3D();
                    ::DestroyWindow(hwnd);
                    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);


                    // close vmm_handle and exit with status: SUCCESS
                    VMMDLL_Close(globals::hVMM);

                    // program completed successfully
                    return EXIT_SUCCESS;
                    return 0;


                
            




}

bool SETUP_Handler()
{
    LPSTR args[] = { (LPSTR)"-device",(LPSTR)"FPGA", (LPSTR)"-norefresh", (LPSTR)"-v" };
    globals::hVMM = VMMDLL_Initialize(3, args);
    if (globals::hVMM)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN); // green
        printf("[OK] ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // default
        printf("  Initialized!\n\n");

        globals::INIT_SUCCESS = true;
    }
    else
    {
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED); // red
        //printf("[ERROR] ");
        //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // default
        //printf("Failure! Try deleting your mmap.txt and re-generating it.\n\n");

        //globals::INIT_SUCCESS = false;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN); // green
        printf("[OK] ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // default
        printf("  Initialized!\n\n");

        globals::INIT_SUCCESS = true;


    }
    return globals::hVMM != NULL;





}

bool GET_Process()
{

        // variable to store function return value (success or fail)
        bool bResult = NULL;;


        // try to get the process id of the 'r5apex.exe' process
        while (true)
        {
            bResult = VMMDLL_PidGetFromName(globals::hVMM, (LPSTR)"crossfire.exe", &globals::dwPID);
            if (bResult)
            {
                break;
            }
        }

        if (bResult)
        {


        }
        else
        {
            // process id could not be retrieved

            return false;
        }



        // pointer to a module entry structure for the 'r5apex.exe' process

        PVMMDLL_MAP_MODULEENTRY pModuleEntryExplorer;
        PVMMDLL_MAP_MODULEENTRY pModuleEntryExplorer2;
        // try to get the base address of the 'r5apex.exe' process
        while (true)
        {
            VMMDLL_Map_GetModuleFromNameU(globals::hVMM, globals::dwPID, (LPSTR)"cshell_x64.dll", &pModuleEntryExplorer, VMMDLL_MODULE_FLAG_NORMAL);
            bResult = VMMDLL_Map_GetModuleFromNameU(globals::hVMM, globals::dwPID, (LPSTR)"crossfire.exe", &pModuleEntryExplorer2, VMMDLL_MODULE_FLAG_NORMAL);
            globals::hS = VMMDLL_Scatter_Initialize(globals::hVMM, globals::dwPID, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO | VMMDLL_FLAG_NOCACHEPUT);
            globals::oS = VMMDLL_Scatter_Initialize(globals::hVMM, globals::dwPID, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING_IO | VMMDLL_FLAG_NOCACHEPUT);
            getwinlogon();
            if (bResult)
            {
                break;
            }
        }

        if (bResult)

        {
            globals::qProcessBase = pModuleEntryExplorer->vaBase;
            globals::qProcessBase2 = pModuleEntryExplorer2->vaBase;

            globals::ProcessStatus = true;
        }
        else
        {

            globals::ProcessStatus = false;
            return false;
        }

        return true;

    
}

void GET_InGame()
{
    char levelName[64];

    // string to store the map name
    std::string sLevelName = "";
    DWORD output=NULL;
    // loop until the cheat is no longer running
    while (globals::CHEAT_RUNNING)
    {

        // read the map name from memory
        if (VMMDLL_MemReadEx(globals::hVMM, globals::dwPID, globals::qProcessBase + offset_renwujizhi + 0x8, (PBYTE)&levelName, sizeof(levelName), &output, VMMDLL_FLAG_NOCACHE | VMMDLL_FLAG_NOPAGING | VMMDLL_FLAG_NOCACHEPUT | VMMDLL_FLAG_NOPAGING_IO))
        {
            // convert the map name to a string
            sLevelName = levelName;
            // printf("%s\n", sLevelName.c_str());
           
             // check if we are in the lobby (indicates we are not in-game)
            if (sLevelName == "" || sLevelName == "mp_lobby")
            {
                globals::IN_GAME = false;
               
                VMMDLL_ConfigSet(globals::hVMM, VMMDLL_OPT_REFRESH_ALL, 1);
                globals::entityInfoVector.clear();              
              
            }
            else
            {
                // not in the lobby, so we must be in-game?
                globals::IN_GAME = true;             

            }
        }

        // sleep for 5 seconds before checking again
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

bool list_any(std::vector<globals::EntityInfo*> list, uintptr_t pEntity)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if ((*it)->pEntity == pEntity) {
            return true;
        }
    }
    return false;
}

void 鬼跳()
{
    int time;
    while (globals::CHEAT_RUNNING)
    {

        if (globals::IN_GAME)
        {
            //update_key_state_bitmap();
            time = aim::guitime;
            if (aim::鬼跳判断)
            {

                if (is_key_down(aim::guikey))
                {
                    if (globals::km)
                    {
                        std::string command = "km.down(" + std::to_string(KEY_SPACEBAR) + ")\r\n"; // left mouse button down
                        Sleep(20); 
                        std::string command1 = "km.up(" + std::to_string(KEY_SPACEBAR) + ")\r\n"; // left mouse button up
                        send_command(hSerial, command.c_str());
                        send_command(hSerial, command1.c_str());

                    }

                    if (globals::kmnet)
                    {
                        kmNet_keydown(KEY_SPACEBAR);
                        Sleep(20);
                        kmNet_keyup(KEY_SPACEBAR);
                    }


                }
            }

        }




        std::this_thread::sleep_for(std::chrono::milliseconds(time));

    }
}

void 碎步()
{
    int time;
    while (globals::CHEAT_RUNNING)
    {

        if (globals::IN_GAME)
        {
            time = aim::suitime;
            if (aim::碎步判断)
            {
                if (is_key_down(aim::suikey))
                {
                    //printf("已按下");
                    if (globals::km)
                    {
                        std::string command = "km.down(" + std::to_string(KEY_W) + ")\r\n"; // left mouse button down
                        Sleep(20);
                        std::string command1 = "km.up(" + std::to_string(KEY_W) + ")\r\n"; // left mouse button up
                        send_command(hSerial, command.c_str());
                        send_command(hSerial, command1.c_str());

                    }

                    if (globals::kmnet)
                    {
                        kmNet_keydown(KEY_W);
                        Sleep(20);
                        kmNet_keyup(KEY_W);
                    }


                }
            }

        }




        std::this_thread::sleep_for(std::chrono::milliseconds(time));

    }
}

void 红名()
{
    int time;
    while (globals::CHEAT_RUNNING)
    {
        //update_key_state_bitmap();
        if (globals::IN_GAME)
        {
            time = aim::bantime;
            if (aim::红名扳机)
            {
               
                
                globals::fov = read<float>(globals::qProcessBase + offset_fov);
                
                if (globals::fov < 1.0f && globals::fov != 0.0f)
                {
                    if (globals::红名 == 257 or globals::红名 == 256)
                    {
                        /*printf("数值：%f\r\n", globals::fov);*/
                        
                        if (globals::km)
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(time));
                            km_click();
                        }

                        if (globals::kmnet)
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(time));
                            kmNet_mouse_left(1);
                            Sleep(5);
                            kmNet_mouse_left(0);
                        }

                    }
                    
                }

            }

        }




        std::this_thread::sleep_for(std::chrono::milliseconds(2));

    }
}

void USP()
{
    int time;
    while (globals::CHEAT_RUNNING)
    {
        
        
        
        //update_key_state_bitmap();
        if (globals::IN_GAME)
        {
            time = aim::USPtime;
            if (aim::USP判断)
            {

                //__int64 武器地址 = read<uintptr_t>(globals::qProcessBase + offset_wuqidizhi);
                __int64 武器ID = read<uintptr_t>(read<uintptr_t>(globals::qProcessBase + offset_renwujizhi) + 0x17AC);
                __int64 武器类别 = read<uintptr_t>(read<uintptr_t>(globals::qProcessBase + offset_renwujizhi) + 0x2E8);

                if (武器类别 == 1)
                {
                    if (取当前武器名(武器ID).find("USP", 0) != std::string::npos)
                    {
                        if (is_key_down(aim::USPkey)) 
                        {
                            if (globals::km)
                            {
                                
                                km_click();
                            }

                            if (globals::kmnet)
                            {
                                
                                kmNet_mouse_left(1);
                                Sleep(5);
                                kmNet_mouse_left(0);

                            }

                        }
                        
                    }


                }
            }




            std::this_thread::sleep_for(std::chrono::milliseconds(time));

        }
    }
}

void thread_getList()
{


    // loop until the cheat is no longer running
    while (globals::CHEAT_RUNNING)
    {
        int 警察 = 0;
        uintptr_t 位置 = 0;
        int 人数 = 0;

        bool 敌人;



        位置 = 取自己位置();
      
        人数 = 取人物数量();     
        if (位置 <= 8)
        {
            警察 = 0;
        }
        else
        {
            警察 = 1;
        }

        if (globals::IN_GAME)
        {
         
            VMMDLL_ConfigSet(globals::hVMM, VMMDLL_OPT_REFRESH_FREQ_MEM, 1);
            VMMDLL_ConfigSet(globals::hVMM, VMMDLL_OPT_REFRESH_FREQ_TLB, 1);
            globals::entityInfoVector.clear();
            globals::shuzu = read<uintptr_t>(globals::qProcessBase + offset_wanjiashuzu);           
            for (int i = 0; i <= 30; i++)
            {
               
                if (警察 == 1)
                {
                    if (i + 1 - 警察 <= 8)
                    {
                        敌人 = true;
                    }
                    else
                    {
                        敌人 = false;
                    }
                }
                if (警察 == 0)
                {
                    if (i + 1 - 警察 > 9)
                    {
                        敌人 = true;
                    }
                    else
                    {
                        敌人 = false;
                    }
                }
                if (人数 >= 16)
                {
                    敌人 = true;
                }
                if (人数 == 15)
                {
                    敌人 =true;
                }
               // printf("警察：%d  位置：%d 人数:%d\n", 警察, 位置, 人数);
                if (敌人 == true)
                {

                    if (i != 位置)
                    {
                       // int health = 0; DWORD output = 0;
                        uintptr_t pEntity = read<uintptr_t>(read<uintptr_t>(globals::qProcessBase + offset_wanjiashuzu) + 0x290 + (i - 1) * offset_shuzudaxiao);                      
                        if (pEntity)
                        {
                           
                            /*
                            if (list_any(globals::entityInfoVector, pEntity)) {
                                continue;
                            }*/

                            globals::EntityInfo* entityInfo = new globals::EntityInfo();
                            entityInfo->pEntity = pEntity;
                            entityInfo->count = i;
                            globals::entityInfoVector.push_back(entityInfo);

                        }
                            
                        
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }

    }
}

void other()
{
    char  bug[50]; 
    while (globals::CHEAT_RUNNING)
    {
       
        if (globals::IN_GAME)
        {
         
            auto handle = CreateScatterHandle();       
            VMMDLL_Scatter_Prepare(handle, offset_juzhen , sizeof(byte) * 64);
            VMMDLL_Scatter_Prepare(handle, offset_juzhen - 64, sizeof(byte) * 64);
            VMMDLL_Scatter_Prepare(handle, offset_juzhen - 128, sizeof(byte) * 64);
            scatterPrep<uintptr_t>(handle, globals::qProcessBase + offset_renwujizhi);
            scatterExec(handle);
            globals::投影[64] = VMMDLL_Scatter_Read(handle, offset_juzhen , sizeof(byte) * 64, (PBYTE)&globals::视口, NULL);
            globals::投影[64] = VMMDLL_Scatter_Read(handle, offset_juzhen - 64, sizeof(byte) * 64, (PBYTE)&globals::投影, NULL);
            globals::观察[64] = VMMDLL_Scatter_Read(handle, offset_juzhen - 128, sizeof(byte) * 64, (PBYTE)&globals::观察, NULL);
            globals::localplayer = scatterRead<uintptr_t>(handle, globals::qProcessBase + offset_renwujizhi);
            scatterClear(handle);
            CloseScatterHandle(handle);
           
        }
       
       
    
       
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
}

void thread_getListInfo()
{
    Vector entityPos;
    Vector entityhead;
    Vector 脖子, 脖子2d;
    Vector 屁股,屁股2d;
    Vector aimpos;
    DWORD output;
    Vector V2A,V2B;
    Vector V3A, V3B;
    int aimselect = aim::aimbone;

    // loop until the cheat is no longer running
    //while (globals::CHEAT_RUNNING)
    {
        // only update the list if we are in-game
        if (globals::IN_GAME)
        {
            globals::红名 = read<uintptr_t>(globals::qProcessBase + offset_hongming);
            //printf("%d\r\n", globals::红名);
            if (aim::crosshair == true)
            {
                DrawCrosshair(ImColor(0, 255, 0, 255));
            }

            if (aim::锁头判断) 
            {
                if (is_key_down(VK_LSHIFT))
                    aimselect = 6;
                else
                    aimselect = aim::aimbone;

            }

            auto handle = CreateScatterHandle();
            for (int i = 0; i < globals::entityInfoVector.size(); i++)
            {

                auto entityInfo = globals::entityInfoVector[i];

                //entityInfo->pEntity = pEntity;
                auto pEntity = entityInfo->pEntity;
                if (!pEntity) { continue; }
               
                AddScatterReadRequest(handle, pEntity+offset_bone, &entityInfo->xx, sizeof(entityInfo->xx));
               // scatterPrep<uintptr_t>(globals::hS, pEntity + offset_bone);             


            }
            ExecuteReadScatter(handle);
            CloseScatterHandle(handle);

            for (int i = 0; i < globals::entityInfoVector.size(); i++)
            {

                auto entityInfo = globals::entityInfoVector[i];

                //entityInfo->pEntity = pEntity;
                auto pEntity = entityInfo->pEntity;
                if (!pEntity) { continue; }



                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 0);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 0 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 0 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 3);//屁股
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 3 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 3 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 6);//头
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 6 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 6 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 5);//脖子
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 5 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 5 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 8);//左臂
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 8 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 8 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 9);//左肘
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 9 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 9 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 10);//左手
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 10 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 10 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 15);//右臂
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 15 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 15 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 16);//右肘
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 16 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 16 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 17);//右手
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 17 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 17 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 21);//左腿
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 21 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 21 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 2);//左膝
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 2 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 2 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 23);//左脚
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 23 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 23 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 25);//右腿
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 25 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 25 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 26);//右膝
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 26 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 26 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 27);//右脚
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 27 + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * 27 + 32);

                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * aimselect);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * aimselect + 16);
                scatterPrep<uintptr_t>(globals::hS, entityInfo->xx + 12 + 64 * aimselect + 32);

                scatterPrep<int>(globals::hS, globals::shuzu + 0x290 + 68 + (entityInfo->count - 1) * offset_shuzudaxiao);
                scatterPrep<int>(globals::hS, globals::shuzu + 0x290 + 40 + (entityInfo->count - 1) * offset_shuzudaxiao);
                scatterPrep<uintptr_t>(globals::hS, globals::shuzu + 0x290 + 24 + (entityInfo->count - 1) * offset_shuzudaxiao);
                VMMDLL_Scatter_Prepare(globals::hS, globals::shuzu + 0x290 + 10 + (entityInfo->count - 1) * offset_shuzudaxiao, sizeof(16));


            }
            scatterExec(globals::hS);
            for (int i = 0; i < globals::entityInfoVector.size(); i++)
            {

                auto entityInfo = globals::entityInfoVector[i];

                
                auto pEntity = entityInfo->pEntity;
                if (!pEntity) { continue; }

               

                取骨骼坐标(entityInfo->count - 1, entityPos, 0, pEntity, entityInfo->xx);               
                entityPos.z = entityPos.z - 140;
                cfworldscreen(entityPos, entityInfo->entityPos);             

                取骨骼坐标(entityInfo->count - 1, entityhead, 6, pEntity, entityInfo->xx);             
                entityhead.z = entityhead.z + 40;
                cfworldscreen(entityhead, entityInfo->entHead);

                取骨骼坐标(entityInfo->count - 1, aimpos, aimselect, pEntity, entityInfo->xx);
                cfworldscreen(aimpos, entityInfo->boxhead);

                entityInfo->entityHealth = scatterRead<int>(globals::hS, globals::shuzu + 0x290 + 68 + (entityInfo->count - 1) * offset_shuzudaxiao);
                int height = entityInfo->entityPos.y - entityInfo->entHead.y;
                int width = height / 2.f;                   
                
                char name[16] = {};
                VMMDLL_Scatter_Read(globals::hS, globals::shuzu + 0x290 + 10 + (entityInfo->count - 1) * offset_shuzudaxiao, sizeof(name),(PBYTE)&name, NULL);            
                int c4 = scatterRead<int>(globals::hS, globals::shuzu + 0x290 + 40 + (entityInfo->count - 1) * offset_shuzudaxiao);
                if(c4==1)
                    aim::color = ImColor(0.0f, 1.0f, 0.0f, 1.0f);
                else
                aim::color = ImColor(static_cast<int>(globals::rectangleColor.x * 255), static_cast<int>(globals::rectangleColor.y * 255), static_cast<int>(globals::rectangleColor.z * 255), static_cast<int>(globals::rectangleColor.w * 255));

               // int 生化状态 = read<int>(scatterRead<uintptr_t>(globals::hS, globals::shuzu + 0x290 + 24 + (entityInfo->count - 1) * offset_shuzudaxiao)+ 0x3C90);
               // printf("生化状态：%d\n", 生化状态);
               // if(aim::生化模式&& 生化状态==0)continue;             
                float entHp = entityInfo->entityHealth;
                if (entHp > 101) entHp = 100;
                float HealthHeightCalc = ((float)entHp / 100) * (float)height;
                            
                if ( entityInfo->entityHealth > 0 && entityInfo->entityHealth < 66666)
                {
                    if (aim::skeletonesp )
                    {
                        取骨骼坐标(entityInfo->count - 1, 脖子, 5, pEntity, entityInfo->xx);
                        if (cfworldscreen(脖子, 脖子2d) == false)
                            continue;

                        取骨骼坐标(entityInfo->count - 1, V3B, 8, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3B, V2B) == false)
                            continue;
                        Line(Vector2D(脖子2d.x, 脖子2d.y), Vector2D(V2B.x, V2B.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3A, 9, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3A, V2A) == false)
                            continue;
                        Line(Vector2D(V2B.x, V2B.y), Vector2D(V2A.x, V2A.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3B, 10, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3B, V2B) == false)
                            continue;
                        Line(Vector2D(V2A.x, V2A.y), Vector2D(V2B.x, V2B.y), aim::color, 2);


                        取骨骼坐标(entityInfo->count - 1, V3A, 15, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3A, V2A) == false)
                            continue;
                        Line(Vector2D(脖子2d.x, 脖子2d.y), Vector2D(V2A.x, V2A.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3B, 16, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3B, V2B) == false)
                            continue;
                        Line(Vector2D(V2A.x, V2A.y), Vector2D(V2B.x, V2B.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3A, 17, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3A, V2A) == false)
                            continue;
                        Line(Vector2D(V2B.x, V2B.y), Vector2D(V2A.x, V2A.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, 屁股, 3, pEntity, entityInfo->xx);
                        if (cfworldscreen(屁股, 屁股2d) == false)
                            continue;
                        Line(Vector2D(脖子2d.x, 脖子2d.y), Vector2D(屁股2d.x, 屁股2d.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3B, 21, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3B, V2B) == false)
                            continue;
                        Line(Vector2D(屁股2d.x, 屁股2d.y), Vector2D(V2B.x, V2B.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3A, 22, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3A, V2A) == false)
                            continue;
                        Line(Vector2D(V2B.x, V2B.y), Vector2D(V2A.x, V2A.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3B, 23, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3B, V2B) == false)
                            continue;
                        Line(Vector2D(V2A.x, V2A.y), Vector2D(V2B.x, V2B.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3A, 25, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3A, V2A) == false)
                            continue;
                        Line(Vector2D(屁股2d.x, 屁股2d.y), Vector2D(V2A.x, V2A.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3B, 26, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3B, V2B) == false)
                            continue;
                        Line(Vector2D(V2A.x, V2A.y), Vector2D(V2B.x, V2B.y), aim::color, 2);

                        取骨骼坐标(entityInfo->count - 1, V3A, 27, pEntity, entityInfo->xx);
                        if (cfworldscreen(V3A, V2A) == false)
                            continue;
                        Line(Vector2D(V2B.x, V2B.y), Vector2D(V2A.x, V2A.y), aim::color, 2);
                    }
                    if (aim::显示头部)
                    DrawCircle(ImVec2(entityInfo->boxhead.x, entityInfo->boxhead.y), abs(width / 5), ImColor(0, 255, 0, 255));
                    if (aim::hp)
                    {
                        DrawFilledRectImGui(entityInfo->entHead.x - (width / 2) - 2, entityInfo->entityPos.y, 3, -height, ImVec4(30.0f, 30.0f, 30.0f, 220.0f));//血量
                        DrawFilledRectImGui(entityInfo->entHead.x - (width / 2) - 2, entityInfo->entityPos.y, 3, -HealthHeightCalc, ImVec4(255.0f, 0, 0, 255.0f));//血量                   
                    }
                    if(aim::boxesp)
                    drawBox(entityInfo->entHead.x - (width / 2), entityInfo->entHead.y, width, height, aim::color, 0.5);
                   

                    if(aim::显示名字)
                    DrawEntityInfo(name, entityInfo->entHead.x, entityInfo->entHead.y);


                    if(aim::范围判断)
                        DrawCircle(ImVec2(globals::ScreenWidth/2+2, globals::ScreenHeight/2+2), (float)aim::fov, ImColor(255, 255, 255, 255));

                }
               
                update_key_state_bitmap();

                

                float dis = Getfov(globals::ScreenWidth / 2, entityInfo->boxhead.x, globals::ScreenHeight / 2, entityInfo->boxhead.y);

                if (entityInfo->entityHealth > 0 && entityInfo->entityHealth < 70000)
                {
                    if (aim::lock == false)
                    {
                        if (dis == 0 || dis < aim::screenfov)
                        {

                            aim::addr = pEntity;
                            aim::count = entityInfo->count - 1;
                            aim::xx = entityInfo->xx;
                            aim::aimpos = entityInfo->boxhead;

                        }

                    }
                    else if (pEntity == aim::addr)
                    {

                        aim::count = entityInfo->count - 1;
                        aim::xx = entityInfo->xx;
                        aim::aimpos = entityInfo->boxhead;

                    }
                }

             
            }
            scatterClear(globals::hS);      
            

            
            if (aim::addr != 0)
            {
                int health = read<int>(globals::shuzu + 0x290 + 68 + (aim::count)*offset_shuzudaxiao);

                if (health == 0)
                {
                    aim::lock = false;
                    aim::screenfov = aim::fov;
                    aim::addr = 0;
                }
                if (aim::aimbot)
                {
                    if (is_key_down(aim::aimkey))
                    {
                        aim::lock = true;
                        startAim(aim::aimpos);
                        // feiaim(aim::addr, aim::aimpos);

                    }
                }
                    if (aim::红名吸附)
                    {

                        if (globals::红名 == 257 or globals::红名 == 256)
                        {
                            aim::lock = true;
                            startAim(aim::aimpos);
                        }
                    }

                else
                {

                    aim::lock = false;
                    aim::screenfov = aim::fov;
                    aim::addr = 0;

                }

            }
            else
            {

                aim::lock = false;
                aim::screenfov = aim::fov;
                aim::addr = 0;

            }

        }
       // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void triggerbot()
{

    Vector entityPos;
    Vector entityhead;

    int height = NULL;
    int width = NULL;
    Vector wdpos = {};
    Vector wdhead = {};
    int isvisable = 0;
    while (globals::CHEAT_RUNNING)
    {
        if (globals::IN_GAME)
        {

            for (int i = 0; i < globals::entityInfoVector.size(); i++)
            {


                auto entityInfo = globals::entityInfoVector[i];

                if (!entityInfo) { continue; }
                //printf("entity: %p\n", entityInfo);
                //printf("hp: %0.f\n", entityInfo->entityHealth);
                //continue;

                auto wdpos = entityInfo->entityPos;
                auto wdhead = entityInfo->entHead;
                auto pEntity = entityInfo->pEntity;
                auto health = entityInfo->entityHealth;

                int height = wdpos.y - wdhead.y;
                int width = height / 2.f;

                bool is_center_in_view = false;
                update_key_state_bitmap();

                if (aim::triggerbot)
                {
                    is_center_in_view = IsCenterInView(ImVec2(entityInfo->boxhead.x, entityInfo->boxhead.y), abs(width / 5));
                    if (is_center_in_view)
                    {
                        if (aim::triggerbotkey != 0)
                        {
                            if (is_key_down(aim::triggerbotkey))
                            {
                                if (globals::km)
                                {
                                    km_click();
                                }

                                if (globals::kmnet)
                                {
                                    kmNet_mouse_left(1);
                                    Sleep(5);
                                    kmNet_mouse_left(0);
                                }
                              
                            }
                        }
                        
                       
                    }
                }
               
                float dis = Getfov(globals::ScreenWidth / 2, entityInfo->boxhead.x, globals::ScreenHeight / 2, entityInfo->boxhead.y);

                if (entityInfo->entityHealth > 0 && entityInfo->entityHealth < 70000)
                {
                    if (aim::lock == false)
                    {
                        if (dis == 0 || dis < aim::screenfov)
                        {
                            
                            aim::addr = pEntity;
                            aim::count = entityInfo->count - 1;
                            aim::aimpos = entityInfo->boxhead;

                        }

                    }
                    else if (pEntity == aim::addr)
                    {

                        aim::count = entityInfo->count - 1;
                        aim::aimpos = entityInfo->boxhead;

                    }                   
                }




            }
            if (aim::红名扳机)
            {
                if (globals::红名 == 257)
                {

                    if (globals::km)
                    {
                        km_click();
                    }

                    if (globals::kmnet)
                    {
                        kmNet_mouse_left(1);
                        Sleep(5);
                        kmNet_mouse_left(0);
                    }
                }

            }

                
            if (aim::addr != 0 )
            {            
                int health = read<int>(globals::shuzu + 0x290 + 68 + (aim::count) * offset_shuzudaxiao);
               
                if (health == 0) 
                {
                    aim::lock = false;
                    aim::screenfov = aim::fov;
                    aim::addr = 0;
                }
                    if (is_key_down(aim::aimkey))
                    {
                        aim::lock = true;

                        feiaim(aim::addr, aim::aimpos);

                    }
                    else
                    {

                        aim::lock = false;
                        aim::screenfov = aim::fov;
                        aim::addr = 0;

                    }
                
            }
            else
            {
               
                aim::lock = false;
                aim::screenfov = aim::fov;
                aim::addr = 0;
              
            }
           
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
   
}