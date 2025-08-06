#include <iostream>
#include "Serializer.hpp"
#include "Data.hpp"

int main() {
    // create a Data instance on the heap
    Data* data = new Data(42, "answer");

    std::cout << "Original pointer:    " << data << "\n";
    std::cout << "Original contents:   id=" << data->id
              << ", name=" << data->name << "\n\n";

    // serialize → uintptr_t
    uintptr_t raw = Serializer::serialize(data);
    std::cout << "Serialized value:    " << raw << "\n\n";

    // deserialize → Data*
    Data* ptr2 = Serializer::deserialize(raw);
    std::cout << "Deserialized pointer:" << ptr2 << "\n";
    std::cout << "Deserialized contents: id=" << ptr2->id
              << ", name=" << ptr2->name << "\n\n";

    // verify equality
    std::cout << "Pointers equal?      "
              << std::boolalpha << (data == ptr2) << "\n";

    delete data;
    return 0;
}
