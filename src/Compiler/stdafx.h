#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <document.h>
#include <filereadstream.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <memory>
#include <clocale>
#include <regex>
#include <filesystem>
#include <stringbuffer.h>
#include <writer.h>
#include <istreamwrapper.h>
#include <functional>
#include <unordered_set>
#include <stack>

#include <Utils.h>

typedef rapidjson::GenericDocument<rapidjson::UTF16<>> WDocument;
typedef rapidjson::GenericValue<rapidjson::UTF16<>> WValue;
typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<>> WStringBuffer;