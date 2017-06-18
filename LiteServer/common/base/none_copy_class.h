#ifndef NONE_COPY_CLASS_H
#define NONE_COPY_CLASS_H


class NoneCopyClass
{
protected:
	NoneCopyClass(){}
	~NoneCopyClass(){}
private:
    NoneCopyClass(const NoneCopyClass&);
    NoneCopyClass& operator = (const NoneCopyClass&);
};

#endif



