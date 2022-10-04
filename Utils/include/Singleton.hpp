

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace Utils
{
    // 饿汉模式，之前已经创造出来这个对象了
    template <typename T>
    class Singleton
    {
    public:
        static T *GetInstance()
        {
            return m_pInstance;
        }
    private:
        static T *m_pInstance;  
        Singleton() {}
    };

    template <typename T>
    T *Singleton<T>::m_pInstance = new T;  // 必须类外初始化
}

#endif // SINGLETON_HPP
