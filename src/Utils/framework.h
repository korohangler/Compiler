#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define THROW_ERROR_IF_FALSE(expression, msg) if(!(expression)) throw std::runtime_error((msg));