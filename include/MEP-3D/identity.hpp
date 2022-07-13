#ifndef IDENTITY_HPP
#define IDENTITY_HPP

#include<unordered_map>
#include<string>

class Identity {
public:
    Identity(const char* name);
    unsigned int GetId() const;
    virtual std::string ToString() const;

private:
    static std::unordered_map<const char*, int> identity_;
    const char* identity_char_;
    unsigned int id_;

};

#endif