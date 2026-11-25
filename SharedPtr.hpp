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

    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        if (refCount_) ++(*refCount_);
    }

    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        other.ptr_ = nullptr;
        other.refCount_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            SharedPtr tmp(other);
            swap(tmp);
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            refCount_ = other.refCount_;
            other.ptr_ = nullptr;
            other.refCount_ = nullptr;
        }
        return *this;
    }

    /** @brief Освобождает объект, если владелец последний. */
    ~SharedPtr() { release(); }

    /**
     * @brief Заменяет управляемый объект.
     * @param p Новый указатель (владение передаётся). nullptr — очистить.
     */
    void reset(T* p = nullptr) {
        if (p != ptr_) {
            release();
            ptr_ = p;
            refCount_ = p ? new std::size_t(1) : nullptr;
            if (p) ++activeObjects_;
        }
    }

    std::size_t use_count() const noexcept {
        return refCount_ ? *refCount_ : 0;
    }

    T* get() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }
    static std::size_t activeCount() noexcept { return activeObjects_; }

    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(refCount_, other.refCount_);
    }

    T& operator*()  const { return *ptr_; }
    T* operator->() const { return ptr_; }

protected:
    void release() noexcept {
        if (refCount_ && --(*refCount_) == 0) {
            delete ptr_;
            delete refCount_;
            --activeObjects_;
        }
        ptr_ = nullptr;
        refCount_ = nullptr;
    }

    T* ptr_;
    std::size_t* refCount_;
    static inline std::size_t activeObjects_ = 0;
};