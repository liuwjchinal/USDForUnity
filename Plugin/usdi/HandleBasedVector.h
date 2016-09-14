#pragma once

#include <vector>
#include <memory>

template<class ValueType>
class HandleBasedVector
{
public:
    typedef ValueType value_t;
    typedef unsigned int handle_t;
    typedef std::vector<value_t> values_t;
    typedef std::vector<handle_t> handles_t;

    HandleBasedVector()
    {
        // 0th element is "null"
        m_values.push_back(value_t());
    }

    ~HandleBasedVector()
    {
    }

    handle_t push(const value_t& v)
    {
        handle_t ret = 0;
        if (m_handles.empty()) {
            ret = (handle_t)m_values.size();
            m_values.emplace_back(v);
        }
        else {
            ret = m_handles.back();
            m_handles.pop_back();
            m_values[ret] = v;
        }
        return ret;
    }

    bool valid(handle_t h) const
    {
        return h != 0 && h < m_values.size();
    }

    value_t& get(handle_t h)
    {
        return m_values[h];
    }

    const value_t& get(handle_t h) const
    {
        return m_values[h];
    }

    value_t pull(handle_t h)
    {
        auto tmp = std::move(m_values[h]);
        m_handles.push_back(h);
        return std::move(tmp);
    }

private:
    values_t m_values;
    handles_t m_handles;
};


// specialization for std::shared_ptr
template<class ValueType>
class HandleBasedVector<std::shared_ptr<ValueType>>
{
public:
    typedef ValueType value_t;
    typedef unsigned int handle_t;
    typedef std::shared_ptr<value_t> pointer_t;
    typedef std::vector<pointer_t> pointers_t;
    typedef std::vector<handle_t> handles_t;

    HandleBasedVector()
    {
        // 0th element is "null"
        m_values.push_back(pointer_t());
    }

    ~HandleBasedVector()
    {
    }

    handle_t push(value_t *v)
    {
        return push(pointer_t(v));
    }

    handle_t push(const pointer_t& v)
    {
        handle_t ret = 0;
        if (m_handles.empty()) {
            ret = (handle_t)m_values.size();
            m_values.emplace_back(v);
        }
        else {
            ret = m_handles.back();
            m_handles.pop_back();
            m_values[ret] = std::move(v);
        }
        return ret;
    }

    bool valid(handle_t h) const
    {
        return h != 0 && h < m_values.size() && m_values[h].get();
    }

    value_t& get(handle_t h)
    {
        return *m_values[h];
    }

    const value_t& get(handle_t h) const
    {
        return *m_values[h];
    }

    pointer_t pull(handle_t h)
    {
        auto tmp = std::move(m_values[h]);
        m_handles.push_back(h);
        return std::move(tmp);
    }

private:
    pointers_t m_values;
    handles_t m_handles;
};
