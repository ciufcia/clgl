#pragma once

#include "utils/utils.hpp"
#include <list>
#include <unordered_map>
#include <memory>
#include <concepts>
#include "exceptions.hpp"
#include "clgl_resource.hpp"

namespace clgl
{
class CLGLResourceManager
{
public:

    typedef U32 ResourceID;

public:

    CLGLResourceManager() = default;

    template<typename T, typename ... Params> requires std::derived_from<T, CLGLResource>
    T *load_resource(ResourceID id = 0, Params&... params);

    template<typename T> requires std::derived_from<T, CLGLResource>
    void load_resource(std::shared_ptr<T> resource, ResourceID id = 0);

    template<typename T> requires std::derived_from<T, CLGLResource>
    void deload_resource(ResourceID id = 0);

    template<typename T> requires std::derived_from<T, CLGLResource>
    T *access_resource(ResourceID id = 0);

private:

    typedef std::list<std::shared_ptr<CLGLResource>> ResourceList;
    typedef std::unordered_map<U32, ResourceList::iterator> IDMappings;
    typedef std::unordered_map<std::size_t, IDMappings> ResourceMap;

    ResourceList m_resources {};
    ResourceMap m_resource_map {};
};

template<typename T, typename ... Params> requires std::derived_from<T, CLGLResource>
T *CLGLResourceManager::load_resource(ResourceID id, Params&... params) {
    std::size_t hash_code = typeid(T).hash_code();
    ResourceMap::iterator resource_map_iter = m_resource_map.find(hash_code);

    if (resource_map_iter == m_resource_map.end())
        resource_map_iter = m_resource_map.insert({ hash_code, IDMappings() }).first;

    IDMappings &id_mappings = resource_map_iter->second;

    IDMappings::iterator id_mappings_iter = id_mappings.find(id);

    if (id_mappings_iter != id_mappings.end())
        throw exceptions::InvalidParameter();

    std::shared_ptr<T> p_resource = std::make_shared<T>(params...);

    p_resource->on_load();

    std::shared_ptr<CLGLResource> p_downcasted_resource = std::dynamic_pointer_cast<CLGLResource>(p_resource);

    m_resources.push_back(p_downcasted_resource);
    ResourceList::iterator resource_iter = std::prev(m_resources.end());

    id_mappings.insert({ id, resource_iter });

    return p_resource.get();
}

template<typename T> requires std::derived_from<T, CLGLResource>
void CLGLResourceManager::load_resource(std::shared_ptr<T> resource, ResourceID id) {
    std::size_t hash_code = typeid(T).hash_code();
    ResourceMap::iterator resource_map_iter = m_resource_map.find(hash_code);

    if (resource_map_iter == m_resource_map.end())
        resource_map_iter = m_resource_map.insert({ hash_code, IDMappings() }).first;

    IDMappings &id_mappings = resource_map_iter->second;

    IDMappings::iterator id_mappings_iter = id_mappings.find(id);

    if (id_mappings_iter != id_mappings.end())
        throw exceptions::InvalidParameter();

    resource->on_load();

    std::shared_ptr<CLGLResource> p_downcasted_resource = std::dynamic_pointer_cast<CLGLResource>(resource);

    m_resources.push_back(p_downcasted_resource);
    ResourceList::iterator resource_iter = std::prev(m_resources.end());

    id_mappings.insert({ id, resource_iter });
}

template<typename T> requires std::derived_from<T, CLGLResource>
void CLGLResourceManager::deload_resource(ResourceID id) {
    std::size_t hash_code = typeid(T).hash_code();
    ResourceMap::iterator resource_map_iter = m_resource_map.find(hash_code);

    if (resource_map_iter == m_resource_map.end())
        throw exceptions::InvalidParameter();

    IDMappings &id_mappings = resource_map_iter->second;

    IDMappings::iterator id_mappings_iter = id_mappings.find(id);

    if (id_mappings_iter == id_mappings.end())
        throw exceptions::InvalidParameter();

    ResourceList::iterator resource_list_iter = id_mappings_iter->second;

    m_resources.erase(resource_list_iter);
    id_mappings.erase(id_mappings_iter);
    if (id_mappings.size() == 0u) m_resource_map.erase(resource_map_iter);
}

template<typename T> requires std::derived_from<T, CLGLResource>
T *CLGLResourceManager::access_resource(ResourceID id) {
    std::size_t hash_code = typeid(T).hash_code();
    ResourceMap::iterator resource_map_iter = m_resource_map.find(hash_code);

    if (resource_map_iter == m_resource_map.end())
        throw exceptions::InvalidParameter();

    IDMappings &id_mappings = resource_map_iter->second;

    IDMappings::iterator id_mappings_iter = id_mappings.find(id);

    if (id_mappings_iter == id_mappings.end())
        throw exceptions::InvalidParameter();

    ResourceList::iterator resource_list_iter = id_mappings_iter->second;

    return std::dynamic_pointer_cast<T>(*resource_list_iter).get();
}
}