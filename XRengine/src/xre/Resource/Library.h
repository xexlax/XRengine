#pragma once
#include "xre\Core\Macros.h"
#include <string>
namespace XRE {
	template <typename T>
	class Library {
    public:
        

        XRef<T> Get(const std::string& filepath);

        
    private:
        std::unordered_map<std::string, XRef<T>> m_Storage;

        XRef<T> Load(const std::string& filepath);
        bool Exists(const std::string& filepath) const;
	};
   


    template<typename T>
    XRef<T> Library<T>::Get(const std::string& path)
    {
        if (m_Storage.find(path) != m_Storage.end()) return m_Storage[path];
        else {
            XRef<T> t = T::Create(path);
            m_Storage[path] = t;
            return t;
        }

    }

   
   

}