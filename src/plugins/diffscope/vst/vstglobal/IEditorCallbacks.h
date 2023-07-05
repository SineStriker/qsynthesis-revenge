#ifndef VSTGLOBAL_IEDITORCALLBACKS_H
#define VSTGLOBAL_IEDITORCALLBACKS_H

class IEditorCallbacks {

public:

    virtual void setDirty() = 0;

    virtual void setError(const char *error) = 0;

    virtual void setStatus(const char *status) = 0;

};

#endif // VSTGLOBAL_IEDITORCALLBACKS_H
