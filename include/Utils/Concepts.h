#pragma once 

template <class T, class U>
concept derived = std::is_base_of_v<U, T>;