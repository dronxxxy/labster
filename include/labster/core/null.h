#pragma once

#define RET_ON_NULL(value) ({typeof(value) __value = value; if (!__value) return; __value; })
#define RET_ON_FALSE(value) RET_ON_NULL(value)
#define NOT_NULL(value) ({typeof(value) __value = value; if (!__value) return NULL; __value; })
#define NOT_FALSE(value) NOT_NULL(value)

