#ifndef NGC_BACKEND_H
#define NGC_BACKEND_H


class Backend {
public:
    virtual ~Backend() = default;

    virtual void Generate() = 0;
};


#endif //NGC_BACKEND_H