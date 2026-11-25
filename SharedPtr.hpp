/**
 * @file SharedPtr.hpp
 * @brief Шаблонный умный указатель с подсчётом ссылок.
 */

#pragma once
#include <cstddef>
#include <type_traits>
#include <utility>
#include <iostream>

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

    /** @brief Берёт владение сырым указателем. */
    explicit SharedPtr(T* p)
        : ptr_(p), refCount_(p ? new std::size_t(1) : nullptr) {
        if (p) ++activeObjects_;
    }

    /** @brief Копирующий конструктор. */
    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        if (refCount_) ++(*refCount_);
    }

    /** @brief Перемещающий конструктор. */
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        other.ptr_ = nullptr;
        other.refCount_ = nullptr;
    }

    /** @brief Копирующее присваивание. */
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) { SharedPtr tmp(other); swap(tmp); }
        return *this;
    }

    /** @brief Перемещающее присваивание. */
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

    /** @brief Деструктор. */
    ~SharedPtr() { release(); }

    /** @brief Заменяет управляемый объект. */
    void reset(T* p = nullptr) {
        if (p != ptr_) {
            release();
            ptr_ = p;
            refCount_ = p ? new std::size_t(1) : nullptr;
            if (p) ++activeObjects_;
        }
    }

    /** @brief Количество владельцев. */
    std::size_t use_count() const noexcept {
        return refCount_ ? *refCount_ : 0;
    }

    /** @brief Сырой указатель. */
    T* get() const noexcept { return ptr_; }

    /** @brief Проверка на непустоту. */
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    /** @brief Количество живых объектов. */
    static std::size_t activeCount() noexcept { return activeObjects_; }

    /** @brief Обмен содержимым. */
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(refCount_, other.refCount_);
    }

    /** @brief Разыменование. */
    T& operator*()  const { return *ptr_; }

    /** @brief Доступ к членам. */
    T* operator->() const { return ptr_; }

protected:
    /** @brief Уменьшает счётчик и освобождает память. */
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

/**
 * @brief Частичная специализация SharedPtr для массивов.
 * @tparam T Тип элемента массива. Запрещены void и функции.
 */
template <typename T>
class SharedPtr<T[]> {
    static_assert(!std::is_void_v<T>, "SharedPtr<void[]> запрещён");
    static_assert(!std::is_function_v<T>, "SharedPtr<функция[]> запрещён");

public:
    /** @brief Создаёт пустой указатель. */
    SharedPtr() noexcept : ptr_(nullptr), refCount_(nullptr) {}

    /** @brief Берёт владение массивом. */
    explicit SharedPtr(T* p)
        : ptr_(p), refCount_(p ? new std::size_t(1) : nullptr) {
        if (p) ++activeObjects_;
    }

    /** @brief Копирующий конструктор. */
    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        if (refCount_) ++(*refCount_);
    }

    /** @brief Перемещающий конструктор. */
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), refCount_(other.refCount_) {
        other.ptr_ = nullptr;
        other.refCount_ = nullptr;
    }

    /** @brief Копирующее присваивание. */
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) { SharedPtr tmp(other); swap(tmp); }
        return *this;
    }

    /** @brief Перемещающее присваивание. */
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_; refCount_ = other.refCount_;
            other.ptr_ = nullptr; other.refCount_ = nullptr;
        }
        return *this;
    }

    /** @brief Деструктор. */
    ~SharedPtr() { release(); }

    /** @brief Заменяет массив. */
    void reset(T* p = nullptr) {
        if (p != ptr_) {
            release();
            ptr_ = p;
            refCount_ = p ? new std::size_t(1) : nullptr;
            if (p) ++activeObjects_;
        }
    }

    /** @brief Доступ к элементу по индексу. */
    T& operator[](std::size_t i) const { return ptr_[i]; }

    /** @brief Сырой указатель. */
    T* get() const noexcept { return ptr_; }

    /** @brief Проверка на непустоту. */
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    /** @brief Количество владельцев. */
    std::size_t use_count() const noexcept { return refCount_ ? *refCount_ : 0; }

    /** @brief Количество живых массивов. */
    static std::size_t activeCount() noexcept { return activeObjects_; }

    /** @brief Обмен содержимым. */
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(refCount_, other.refCount_);
    }

protected:
    /** @brief Освобождает массив через delete[]. */
    void release() noexcept {
        if (refCount_ && --(*refCount_) == 0) {
            delete[] ptr_;
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

/**
 * @brief Безопасно создаёт SharedPtr<T> для одиночного объекта.
 */
template <typename T, typename... Args>
std::enable_if_t<
    !std::is_array_v<T> && !std::is_void_v<T> && !std::is_function_v<T>,
    SharedPtr<T>>
make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

/**
 * @brief Безопасно создаёт SharedPtr<T[]> для массива.
 * Специально сделано для вызова make_shared<int[]>(N).
 */
template <typename T>
std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, SharedPtr<T>>
make_shared(std::size_t n) {
    using ElementType = std::remove_extent_t<T>;
    return SharedPtr<T>(new ElementType[n]());
}