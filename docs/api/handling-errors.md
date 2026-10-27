# Handling Errors

The `Dynamicus.hxx` header provides its own status code (`dynamicus::StatusCode`) and result 
(`dynamicus::Status<T>`) structures that are used to indicate either operation failure or success. They are
usually used within Dynamiscus. These structures are used for simillar purposes, but used in different 
situations:

- Use `dynamicus::StatusCode` if the function does not return any values and needs only to report the
  operation's status.

- Use `dynamicus::Status<T>` if the function is intended to return values along with the operation's
  status.


# Example

Example of using `dynamicus::StatusCode`:

```cxx
#include <Dynamicus.hxx>
#include <iostream>

enum class Codes : dynamicus::StatusCode::Type {
  NOT_DIVIDABLE = dynamicus::StatusCode::OK + 1,
};

dynamicus::StatusCode isDividable(int a, int b) {
  if (b == 0) {
    // It is recommended to use fromEnum method to prevent boilerplate type
    // conversions at compilation time. Here, fromEnum checks whether the provided
    // value is really an enumeraiton and its underlying type is
    // dynamicus::StatusCode::Type.
    return dynamicus::StatusCode::fromEnum(Codes::NOT_DIVIDABLE);
  }
  return dynamicus::StatusCode(dynamicus::StatusCode::OK);
}

int main() {
  auto status = isDividable(10, 0);
  if (!status) {
    std::cout << "Unfortunately, 10 / 0 is not dividable..." << std::endl;
  }

  status = isDividable(10, 2);
  if (status) {
    std::cout << "... but 10 / w2 is!" << std::endl;
  }

  return 0;
}
```

Output:
```
Unfortunately, 10 / 0 is not dividable...
... but 10 / 2 is!
```

- Example of using `dynamicus::Status<T>`:

```cxx
#include <Dynamicus.hxx>
#include <iostream>

enum class Codes : dynamicus::StatusCode::Type {
  NOT_DIVIDABLE = dynamicus::StatusCode::OK + 1,
};

dynamicus::Status<int> divide(int a, int b) {
  if (b == 0) {
    // It is recommended to use fromEnum method to prevent boilerplate type
    // conversions at compilation time. Here, fromEnum checks what the provided
    // value is really an enumeraiton and its underlying type is
    // dynamicus::StatusCode::Type.
    return dynamicus::StatusCode::fromEnum(Codes::NOT_DIVIDABLE);
  }
  return a / b;
}

int main() {
  auto result = divide(10, 0);
  if (!result.getCode()) {
    std::cout << "Unfortunately, 10 / 0 is not dividable..." << std::endl;
  }

  result = divide(10, 2);
  if (result.getCode()) {
    std::cout << "... but 10 / 2 is! The division result is "
              << result.getValue().value() << std::endl;
  }

  return 0;
}
```

Output:
```
Unfortunately, 10 / 0 is not dividable...
... but 10 / 2 is! The division result is 5
```

