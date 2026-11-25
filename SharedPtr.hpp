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

    /**
     * @brief Копирующий конструктор: увеличивает счётчик ссылок.
     * @param other Другой SharedPtr.
     */
    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        if (refCount_) ++(*refCount_);
    }

    /**
     * @brief Перемещающий конструктор: забирает владение.
     * @param other SharedPtr, который станет пустым.
     */
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        other.ptr_ = nullptr;
        other.refCount_ = nullptr;
    }

    /**
     * @brief Копирующее присваивание (copy-and-swap).
     * @param other Другой SharedPtr.
     * @return *this.
     */
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            SharedPtr tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief Перемещающее присваивание.
     * @param other SharedPtr, который станет пустым.
     * @return *this.
     */
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

    /**
     * @brief Обменивает содержимое с другим SharedPtr.
     * @param other Другой SharedPtr.
     */
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(refCount_, other.refCount_);
    }

    /// @brief Разыменование.
    T& operator*()  const { return *ptr_; }

    /// @brief Доступ к членам объекта.
    T* operator->() const { return ptr_; }

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