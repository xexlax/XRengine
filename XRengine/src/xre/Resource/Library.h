#pragma once
#include "xre\Core\Macros.h"
#include <string>
namespace XRE {
	template <typename T>
	class Library {
    public:
        

        Ref<T> Get(const std::string& filepath);

        
    private:
        std::unordered_map<std::string, Ref<T>> m_Storage;

        Ref<T> Load(const std::string& filepath);
        bool Exists(const std::string& filepath) const;
	};
   


    template<typename T>
    Ref<T> Library<T>::Get(const std::string& path)
    {
        if (m_Storage.find(path) != m_Storage.end()) return m_Storage[path];
        else {
            Ref<T> t = T::Create(path);
            m_Storage[path] = t;
            return t;
        }

    }

   
   

}