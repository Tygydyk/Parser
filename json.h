#include <any>
#include <iostream>
#include <utility> // for pair<>
#include <fstream>
#include <cctype>
#include <string>
#include <map>
#include <vector>

struct Object{
    std::map<std::string, std::any> field_obj;
};

struct Array{
    std::vector<std::any> field_arr;
};

class Json {
    Object object;
    Array vec;

public:
    Json();

    explicit Json(const std::vector<std::any>& v); //Конструктор с параметром
    //массива полю Json
    // Конструктор из строки, содержащей Json-данные.
    explicit Json(const std::string& s);

    //Метод возвращает true, если данный экземпляр содержит в себе JSON-массив
    //Иначе false.
    bool is_array() const;

    // Метод возвращает true, если данный экземпляр содержит в себе JSON-объект
    //Иначе false.
    bool is_object() const;

    // Метод возвращает значение по ключу key,
    //если экземпляр является JSON-объектом.
    // Значение может иметь один из следующих типов:
    //Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-массивом, генерируется исключение.
    std::any& operator[](const std::string& key);

    // Метод возвращает значение по индексу index,
    //если экземпляр является JSON-массивом.
    // Значение может иметь один из следующих типов:
    //Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-объектом, генерируется исключение.
    std::any& operator[](size_t index);

    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static Json parse(const std::string& s);

    // Метод возвращает объекта класса Json из файла,
    //содержащего Json-данные в текстовом формате.
    static Json parseFile(const std::string& path_to_file);
};

Json::Json() {}

Json::Json(const std::vector<std::any>& v) {
    vec.field_arr = v;
}

Json::Json(const std::string & s) {
    int i = 0;
    std::string key = "", value1 = "";
    std::any value;
    while (s[i] != '\0') {
        if (s[i] == '"') {
            ++i;
            while (s[i] != '"') {
                key += s[i];
                ++i;
            }
        }
        if (s[i] == ':') {
            ++i;
            while (s[i] == ' ') {
                ++i;
            }
            if (s[i] == '"') {
                ++i;
                while (s[i] != '"') {
                    value1 += s[i];
                    ++i;
                }
                object.field_obj.insert(std::pair<std::string,
                std::string>(key, value1));
                key = value1 = "";
            } else {
                if (s[i] == '[') {
                    std::vector<std::any> v;
                    while (s[i] != ']') {
                        ++i;
                        while (s[i] == ' ') {
                            ++i;
                        }
                        if (isdigit(s[i])) {
                            v.push_back(atof(&(s[i])));
                            while (s[i] != ',' && s[i] != ']') {
                                ++i;
                            }
                        } else {
                            if (s[i] == '"') {
                                ++i;
                                std::string s_arr = "";
                                while (s[i] != '"') {
                                    s_arr += s[i];
                                    ++i;
                                }
                                v.push_back(s_arr);
                                } else {
                                if (s[i] == '[') {
                                    ++i;
                                    std::string s_mas = "";
                                    while (s[i] != ']'){
                                        s_mas += s[i];
                                        ++i;
                                    }
                                    s_mas += " ";
                                    ++i;
                                    v.push_back(Json(s_mas));
                                } else {
                                    if (s[i] == '{') {
                                        ++i;
                                        std::string str = "";
                                        while (s[i] != '}'){
                                            str += s[i];
                                            ++i;
                                        }
                                        v.push_back(Json(str));
                                    }
                                }
                            }
                        }
                    }
                    Json temp_arr(v);
                    value = temp_arr;
                    object.field_obj.insert(std::pair<std::string,
                    std::any>(key, value));
                } else {
                    if (s[i] == '{') {
                        std::string temp_s;
                        while (s[i] != '}') {
                            temp_s += s[i];
                            ++i;
                        }
                        temp_s += '}';
                        Json temp(temp_s);
                        object.field_obj.insert(
                            std::pair<std::string, Json>(key, temp));
                    } else {
                        if (isdigit(s[i])) {
                            value = atof(&(s[i]));
                            object.field_obj.insert(
                                std::pair<std::string, std::any>(key, value));
                        } else {
                            if (s[i] == 'f') {
                            std::string s_bool = "";
                            while (s[i] != ',' && s[i] != '"' && s[i] != ' ') {
                                    s_bool += s[i];
                                    ++i;
                                }
                                if (s_bool == "false") {
                                    value = false;
                                    object.field_obj.insert(
                                std::pair<std::string, std::any>(key, value));
                                }
                            } else {
                                if (s[i] == 't') {
                                    std::string s_bool = "";
                                    while (s[i] != ',') {
                                        s_bool += s[i];
                                        ++i;
                                    }
                                    if (s_bool == "true") {
                                    value = true;
                                    object.field_obj.insert(
                                std::pair<std::string, std::any>(key, value));
                                    }
                                } else {
                                if (s[i] == 'n') {
                                std::string s_bool = "";
                            while (s[i] != ',' && s[i] != ' ' && s[i] != '"') {
                                            s_bool += s[i];
                                            ++i;
                                        }
                                        if (s_bool == "null") {
                                        value = NULL;
                                        object.field_obj.insert(
                                std::pair<std::string, std::any>(key, value));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                key = "";
            }
        }
        ++i;
    }
}

bool Json::is_array() const {
    return !vec.field_arr.empty();
}

bool Json::is_object() const{
    return !object.field_obj.empty();
}

std::any & Json::operator[](const std::string & key) {
    if (this->is_object()) {
        for (auto it = object.field_obj.begin();
        it != object.field_obj.end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }
    }
    throw std::runtime_error("it`s not an object");
}

std::any & Json::operator[](size_t index) {
    if (this->is_array()) {
        if (index < vec.field_arr.size()) {
            return this->vec.field_arr[index];
        }
    }
    throw std::runtime_error("it`s not an array");
}

Json Json::parse(const std::string & s) {
    return Json(s);
}

Json Json::parseFile(const std::string & path_to_file) {
    std::ifstream fin(path_to_file);
    char temp;
    std::string s;
    if (fin.is_open()) {
        while (!fin.eof()) {
            fin >> temp;
            s += temp;
        }
    } else {
        std::cout << "Данного файла не существует." << std::endl;
    }
    fin.close();
    return Json(s);
}
