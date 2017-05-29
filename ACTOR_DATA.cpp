#include "actor_data.h"
#include <string>
#include <regex>
#include <iostream>
using namespace std;

string ACTOR_DATA::get_pattern(string page, string pattern){
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern);
    sregex_iterator it(page.begin(), page.end(), r);
    string str = it->str();
    return str;
}

char* ACTOR_DATA::turn_to_char(string str){
    cout<<str.size()<<endl;
    char *tmp = new char[str.size()+1];
    cout<<str.c_str()<<endl;
    memcpy(tmp, str.c_str(), str.size() + 1);
    cout<<tmp<<endl;
    return tmp;
}

char *ACTOR_DATA::get_keyinfo(string str){
    char *tmp = turn_to_char(str);
    return get_keyinfo(tmp);
}

char *ACTOR_DATA::get_keyinfo(char *src){  //获得><之间的信息
    while(*src)
    {
        if(*src == '>')
            break;
        src ++;
    }
    src ++;
    char *fp = new char[20];
    char *it = fp;
    while (*src) {
        if(*src == '<')
            break;
        if ((*src != ' ') && (*src != '\n') && (*src != '\r')) { // 如果不是空格就复制
            *it = *src;
            it++;
        }
        src++;
    }
    *it = '\0' ; //封闭字符串
    cout<<fp<<endl;
    return fp;
}

void ACTOR_DATA::get_data(string url){
    string info = get_pattern(url, "<div id=\"headline\" class=\"item\">(.|\\r|\\n)*?<div class=\"info\">(.|\\r|\\n)*?</div>");
    deal_info(info);
}

void ACTOR_DATA::deal_info(string info){
    //获取演员名
    string name = get_pattern(info, "class=\"nbg\" title=\".*?\"");
    name = name.replace(0, 19, "");
    char *n = turn_to_char(name);
    unsigned long pos = name.find("\"");
    n[pos] = '\0';
    actor->name = n;
    cout<<n<<endl;
    //获取img地址
    string img = get_pattern(info, "http.*?large.*?.jpg");
    actor->imgurl = turn_to_char(img);

    //获取演员信息
    match_tag(info);
    
}

void ACTOR_DATA::match_tag(string info){
    string pattern("<li>(.|\\r|\\n)*?</li>");
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern);
    for(sregex_iterator it(info.begin(), info.end(), r), end; it != end; ++it){
        char *tag = get_tag(it->str());
        char *content = get_content(it->str());
        match(tag, content);
    }
}

char* ACTOR_DATA::get_tag(string li){
    string n1 = get_pattern(li, "span>.*?</span");
    char *tag = get_keyinfo(n1);
    return tag;
}

char* ACTOR_DATA::get_content(string li){ //找</span>: * <li>之间的信息
    unsigned long pos = li.find(":");
    li = li.replace(0, pos + 1, "");
    char *con = turn_to_char(li);
    char *fp = new char[20];
    char *it = fp;
    while (*con) {
        if(*con == '<')
            break;
        if ((*con != ' ') && (*con != '\n') && (*con != '\r')) { // 如果不是空格就复制
            *it = *con;
            it++;
        }
        con++;
    }
    *it = '\0' ; //封闭字符串
    return fp;
}

void ACTOR_DATA::match(char *tag, char *content){
    if(strcmp(tag, "性别") == 0)
    {
        actor->sex = content;
        return;
    }
    if(strcmp(tag, "星座")){
        actor->constellation = content;
        return;
    }
    if(strcmp(tag, "出生日期")){
        actor->birthday = content;
        return;
    }
    if(strcmp(tag, "出生地")){
        actor->birthplace = content;
        return;
    }
    if(strcmp(tag, "职业")){
        actor->occupation = content;
        return;
    }
    if(strcmp(tag, "更多外文名")){
        actor->foreignName = content;
        return;
    }
    if(strcmp(tag, "更多中文名")){
        actor->ChineseName = content;
        return;
    }
    if(strcmp(tag, "家庭成员")){
        actor->familyMember = content;
        return;
    }
    if(strcmp(tag, "imdb编号")){
        get_imdbCode(content);
        return;
    }
    if(strcmp(tag, "官方网站")){
        get_selfPage(content);
        return;
    }
}

void ACTOR_DATA::get_imdbCode(string imdb){
    string code = get_pattern(imdb, "nm(\\d)*");
    actor->imdbCode = turn_to_char(code);
}
void ACTOR_DATA::get_selfPage(string page){
    string url = get_pattern(page, "http[^\"]*");
}
