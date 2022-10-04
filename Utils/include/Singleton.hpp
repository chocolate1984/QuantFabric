

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace Utils
{
    // ����ģʽ��֮ǰ�Ѿ�����������������
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
    T *Singleton<T>::m_pInstance = new T;  // ���������ʼ��
}

#endif // SINGLETON_HPP
