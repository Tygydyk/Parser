#include <any>
#include <vector>
#include <string>
#include <cstdlib>

class Json
{
public:
	std::vector<std::any> key_code;
	std::vector<std::any> mean_code;
	std::vector<std::any> array_code;
	// Конструктор из строки, содержащей Json-данные.
	Json(const std::string& str);
	// Метод возвращает true, если данный экземпляр содержит в себе JSON-массив.
	// Иначе false.
	bool is_array() const;
	// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект.
	// Иначе false.
	bool is_object() const;
	// Метод возвращает значение по ключу key, если экземпляр является
	// JSON-объектом.
	// Значение может иметь один из следующих типов: Json, std::string, double,
	// bool или быть пустым.
	// Если экземпляр является JSON-массивом, генерируется исключение.
	std::any& operator[](const std::string& key);
	// Метод возвращает значение по индексу index, если экземпляр является
	// JSON-массивом.
	// Значение может иметь один из следующих типов: Json, std::string, double,
	// bool или быть пустым.
	// Если экземпляр является JSON-объектом, генерируется исключение.
	std::any& operator[](int index);
	// Метод возвращает объект класса Json из строки, содержащей Json-данные.
	static Json parse(const std::string& s)
	{
		Json curr(s);
		return curr;
	};
	// Метод возвращает объекта класса Json из файла, содержащего Json-данные в
	// текстовом формате.
	static Json parseFile(const std::string& path_to_file);
};

Json::Json(const std::string& str)
{
	std::string s;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] != ' ')
		{
			if (str[i] == '\n')
				continue;
			if (str[i] == '\t')
				continue;
			if (str[i] == ',')
			{
				s += str[i];
				s += ' ';
				continue;
			}
			s += str[i];
		}

		else
		{
			if ((str[i - 1] == ':') || (str[i + 1] == ':'))
			{
				s += str[i];
				continue;
			}
			continue;
		}
	}
	int count = 0;

	switch (s[count])
	{
	case '{':
	{
		while (s[count] != '}')
		{
			std::string key_str;
			std::string mean_str;
			while (s[count] != '"')
				count++;
			count++;
			while (s[count] != '"')
			{
				key_str += s[count];
				count++;
			}
			key_code.push_back(key_str);
			for (int i = 0; i < 4; i++)
				count++;

			if (s[count] == '{')
			{
				while (s[count] != '}')
				{
					mean_str += s[count];
					count++;
				}
				mean_str += s[count];
				count++;
				Json mean(mean_str);
				mean_code.push_back(mean);
				continue;
			}

			if (s[count] == '[')
			{
				while (s[count] != ']')
				{
					mean_str += s[count];
					count++;
				}
				mean_str += s[count];
				count++;
				Json mean(mean_str);
				mean_code.push_back(mean);
				continue;
			}

			if (s[count] == '"')
			{
				count++;
				while (s[count] != '"')
				{
					mean_str += s[count];
					count++;
				}
				count++;
				mean_code.push_back(mean_str);
				continue;
			}
			
			if ((s[count] == '1') || (s[count] == '2') || (s[count] == '3') || (s[count] == '4')
				|| (s[count] == '5') || (s[count] == '6') || (s[count] == '7')
				|| (s[count] == '8') || (s[count] == '9') || (s[count] == '0'))
			{
				while ((s[count] != ',') && (s[count] != '}'))
				{
					mean_str += s[count];
					count++;
				}
				double mean = atof(mean_str.c_str());
				mean_code.push_back(mean);
				continue;
			}
		}
	}
	break;

	case '[':
	{
		std::string mean_str;
		count++;

		if (s[count] == '{')
		{
			while (s[count] != ']')
			{
				while (s[count] != '}')
				{
					mean_str += s[count];
					count++;
				}
				mean_str += s[count];
				count++;
				Json mean(mean_str);
				array_code.push_back(mean);
				mean_str = "";
				if (s[count] == ',')
				{
					count++;
					count++;
				}
			}
		}

		if (s[count] == '[')
		{
			while (s[count] != ']')
			{
				while (s[count] != ']')
				{
					mean_str += s[count];
					count++;
				}
				mean_str += s[count];
				count++;
				if (s[count] == ',')
				{
					count++;
					count++;
				}
				Json mean(mean_str);
				array_code.push_back(mean);
				mean_str = "";
			}
		}

		if (s[count] == '"')
		{
			while (s[count] != ']')
			{
				count++;
				while (s[count] != '"')
				{
					mean_str += s[count];
					count++;
				}
				count++;
				if (s[count] == ',')
				{
					count++;
					count++;
				}
				array_code.push_back(mean_str);
				mean_str = "";
			}
		}
		
		if ((s[count] == '1') || (s[count] == '2') || (s[count] == '3') || (s[count] == '4')
			|| (s[count] == '5') || (s[count] == '6') || (s[count] == '7') || (s[count] == '8')
			|| (s[count] == '9') || (s[count] == '0'))
		{
			while (s[count] != ']')
			{
				while ((s[count] != ',') && (s[count] != ']'))
				{
					mean_str += s[count];
					count++;
				}
				double mean = atof(mean_str.c_str());
				array_code.push_back(mean);
				mean_str = "";
				if (s[count] == ',')
				{
					count++;
					count++;
				}
			}
		}
	}
	break;
	}
}

bool Json::is_array() const
{
	if (array_code.size() > 0)
		return true;
	for (unsigned int i = 0; i < mean_code.size(); i++)
	{
		if (mean_code[i].type() == typeid(Json))
		{
			Json json = std::any_cast<Json>(mean_code[i]);
			if (json.is_array())
				return true;
		}
	}
	return false;
}

bool Json::is_object() const
{
	if (mean_code.size() > 0)
		return true;
	for (unsigned int i = 0; i < array_code.size(); i++)
	{
		if (array_code[i].type() == typeid(Json))
		{
			Json json = std::any_cast<Json>(array_code[i]);
			if (json.is_object())
				return true;
		}
	}
	return false;
}

std::any& Json::operator[](const std::string& key)
{
	if (array_code.size() > 0)
	{
		throw 1;
	}
	for (unsigned int i = 0; i < key_code.size(); i++)
	{
		if (std::any_cast<std::string>(key_code[i]) == key)
			return mean_code[i];
	}
	return mean_code[0];
}

std::any& Json::operator[](int index)
{
	if (mean_code.size() > 0)
	{
		throw 1;
	}
	return array_code[index];
}
