#pragma once

#include <ranges>
#include "matrix.hpp"
#include "strided_iterator.hpp"

namespace detail
{
    template <typename T>
    class row_view_impl : public std::ranges::view_interface<row_view_impl<T>>
    {
      public:
        row_view_impl(matrix<T>* m, size_t row) : matrix_(m), row_(row) { }
        
        auto begin() const { return (*matrix_)[row_]; }
        
        auto end() const { return begin() + matrix_->num_columns(); }
      
      private:
        matrix<T>* matrix_;
        size_t row_;
    };
    
    template <typename T>
    class crow_view_impl : public std::ranges::view_interface<crow_view_impl<T>>
    {
      public:
        crow_view_impl(const matrix<T>* m, size_t row) : matrix_(m), row_(row) { }
        
        auto begin() const { return (*matrix_)[row_]; }
        
        auto end() const { return begin() + matrix_->num_columns(); }
      
      private:
        const matrix<T>* matrix_;
        size_t row_;
    };
    
    
    struct row_view_object
    {
        template <typename T>
        row_view_impl<T> operator()(matrix<T>* m, size_t row) const
        {
            return row_view_impl(m, row);
        }
        
        template <typename T>
        crow_view_impl<T> operator()(const matrix<T>* m, size_t row) const
        {
            return crow_view_impl(m, row);
        }
    };
}

constexpr detail::row_view_object row_view;

namespace detail
{
    template <typename T>
    class ccolumn_view_impl : public std::ranges::view_interface<ccolumn_view_impl<T>>
    {
      public:
        ccolumn_view_impl(const matrix<T>* m, size_t column) : matrix_(m), column_(column) { }
        
        auto begin() const { return strided_iterator<const T>(matrix_->data() + column_, matrix_->num_rows()); }
        
        auto end() const { return begin() + matrix_->num_rows(); }
        
      private:
        const matrix<T>* matrix_;
        size_t column_;
    };
    
    
    template <typename T>
    class column_view_impl : public std::ranges::view_interface<ccolumn_view_impl<T>>
    {
      public:
        column_view_impl(matrix<T>* m, size_t column) : matrix_(m), column_(column) { }
        
        auto begin() const { return strided_iterator<T>(matrix_->data() + column_, matrix_->num_rows()); }
        
        auto end() const { return begin() + matrix_->num_rows(); }
      
      private:
        matrix<T>* matrix_;
        size_t column_;
    };
    
    struct column_view_object
    {
        template <typename T>
        column_view_impl<T> operator()(matrix<T>* matrix, size_t column) const
        {
            return column_view_impl(matrix, column);
        }
        
        template <typename T>
        ccolumn_view_impl<T> operator()(const matrix <T>* matrix, size_t column) const
        {
            return ccolumn_view_impl(matrix, column);
        }
    };
}

constexpr detail::column_view_object column_view;

auto rows(auto&& mat)
{
    return std::views::iota(size_t(0), mat.num_rows()) |
           std::views::transform([&](size_t r) { return row_view(&mat, r); });
}

auto columns(auto&& mat)
{
    return std::views::iota(size_t(0), mat.num_columns()) |
           std::views::transform([&](size_t r) { return column_view(&mat, r); });
}