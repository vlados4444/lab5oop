/**
 * @file SharedPtr.hpp
 * @brief Шаблонный умный указатель с подсчётом ссылок.
 */

#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
#include <iostream>
#include <cmath>
#include <typeinfo>

/**
 * @brief Умный указатель с подсчётом ссылок для одиночных объектов.
 * @tparam T Тип управляемого объекта. Запрещены void и функции.
 */
template <typename T>
class SharedPtr {
    static_assert(!std::is_void_v<T>, "SharedPtr<void> запрещён");
    static_assert(!std::is_function_v<T>, "SharedPtr<функция> запрещён");

public:
    /** @brief Создаёт пустой указатель. */
    SharedPtr() noexcept : ptr_(nullptr), refCount_(nullptr) {}

    /**
     * @brief Берёт владение сырым указателем.
     * @param p Указатель из new. Может быть nullptr.
     */
    explicit SharedPtr(T* p)
        : ptr_(p), refCount_(p ? new std::size_t(1) : nullptr) {
        if (p) ++activeObjects_;
    }

    /** @brief Освобождает объект, если владелец последний. */
    ~SharedPtr() { release(); }

    /// @return Количество владельцев; 0, если пуст.
    std::size_t use_count() const noexcept {
        return refCount_ ? *refCount_ : 0;
    }

    /// @return Сырой указатель без передачи владения.
    T* get() const noexcept { return ptr_; }

    /// @return true, если указатель не пуст.
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    /// @return Количество живых управляемых объектов типа T.
    static std::size_t activeCount() noexcept { return activeObjects_; }

protected:
    /// @brief Уменьшает счётчик и при необходимости освобождает память.
    void release() noexcept {
        if (refCount_ && --(*refCount_) == 0) {
            delete ptr_;
            delete refCount_;
            --activeObjects_;
        }
        ptr_ = nullptr;
        refCount_ = nullptr;
    }

    T* ptr_;                 ///< Управляемый объект.
    std::size_t* refCount_;  ///< Разделяемый счётчик ссылок.
    static inline std::size_t activeObjects_ = 0;  ///< Живых объектов T.
};