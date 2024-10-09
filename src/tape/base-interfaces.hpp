namespace BaseInterfaces {

class IBase {
protected:
    virtual ~IBase() = default;

public:
    virtual void Delete() = 0;
    IBase& operator=(const IBase&) = delete;
};

} // namespace BaseInterfaces
