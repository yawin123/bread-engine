#pragma once

#include <utility>
#include <vector>

namespace brd {
  namespace core {
    using index_type = std::size_t;

    template<typename T>
    class slotmap {
      index_type Capacity {10};

      public:
        explicit slotmap() {
          indices_.resize(Capacity);
          data_.reserve(Capacity);
          erase_.reserve(Capacity);
          free_ = 0;

          configureSlots_();
        }

        [[nodiscard]] constexpr T& operator[](index_type id) noexcept
        {
          return data_[indices_[id]];
        }

        [[nodiscard]] constexpr const T& at(index_type id) const noexcept
        {
          return data_[indices_[id]];
        }

        [[nodiscard]] constexpr T& at(index_type id) noexcept
        {
          return data_[indices_[id]];
        }

        [[nodiscard]] constexpr index_type push(T&& e) noexcept
        {
          if(free_ >= indices_.size()) //No quedan huecos
          {
            allocate_();
          }

          //Recogemos el identificador del primer slot libre
            index_type id = free_;

          //Obtenemos el siguiente slot libre
            free_ = indices_[free_];

          //Obtenemos la dirección en data que vamos a usar
            indices_[id] = data_.size();

          //Alojamos nuestro dato
            data_.push_back(std::move(e));
            erase_.push_back(id);

          return id;
        }

        constexpr bool erase(index_type id) noexcept
        {
          if(id >= indices_.size() || indices_[id] >= data_.size()) return false;

          if(indices_[id] < data_.size()-1)
          {
            data_[indices_[id]] = std::move(data_.back());
            erase_[indices_[id]] = std::move(erase_.back());
            indices_[erase_[indices_[id]]] = id;
          }

          data_.erase(data_.end()-1);
          erase_.erase(erase_.end()-1);

          return true;
        }

        constexpr const std::vector<T>& data() const noexcept
        {
          return data_;
        }

        constexpr std::vector<T>& data() noexcept
        {
          return data_;
        }

        [[nodiscard]] constexpr const index_type size() const noexcept
        {
          return data_.size();
        }

        [[nodiscard]] constexpr const index_type capacity() const noexcept
        {
          return indices_.size();
        }

      private:
        constexpr void allocate_() noexcept
        {
          index_type tam = indices_.size()*2;
          indices_.resize(tam);
          data_.reserve(tam);
          erase_.reserve(tam);

          configureSlots_();
        }

        constexpr void configureSlots_() noexcept
        {
          for(int i = free_; i < indices_.size(); ++i)
          {
            indices_[i] = i+1;
          }
        }

        index_type free_{};

        std::vector<index_type> indices_ {};
        std::vector<T> data_ {};
        std::vector<index_type> erase_{};
    };
  };
};
