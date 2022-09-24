#pragma once
#include <random>
#include <iostream>
#include <thread>
#include <cassert>
#include <algorithm>

namespace nev
{
	int getRandomInt(int min, int max);
	double getRandomDouble(double min, double max);
	bool getRandomBool();
	std::mt19937& getRNG();

	/// Returns a random index of a vector
	template <typename T>
	int getRandomIndex(const T& vec)
	{
		assert(!vec.empty());
		return getRandomInt(0, vec.size() - 1);
	}

	template <typename T>
	T getRandomElement(const std::vector<T>& vec)
	{
		return vec[getRandomIndex(vec)];
	}

	template <class T>
	class sorted_vector
	{
	public:
		sorted_vector() = default;
		sorted_vector(std::vector<T>&& vec)
		{
			m_data = std::move(vec);
			std::sort(m_data.begin(), m_data.end());
		}
		sorted_vector(const std::vector<T>& vec)
		{
			m_data = vec;
			std::sort(m_data.begin(), m_data.end());
		}

		// Every element gets inserted in order,
		// however with the [] operator the user can change the key value (of elements) after which the vector is sorted
		// Meaning if this value(s) changes, the user needs to call sort himself
		void sort()
		{
			std::sort(m_data);
		}

		size_t size() const
		{
			return m_data.size();
		}

		const std::vector<T>& getVector() const
		{
			return m_data;
		}

		bool empty() const
		{
			return m_data.empty();
		}

		T& operator[](size_t index)
		{
			return m_data[index];
		}

		const T& operator[](size_t index) const
		{
			return m_data[index];
		}

		auto begin()
		{
			return m_data.begin();
		}

		auto begin() const
		{
			return m_data.begin();
		}

		auto cbegin() const
		{
			return m_data.cbegin();
		}

		auto end()
		{
			return m_data.end();
		}

		auto end() const
		{
			return m_data.end();
		}

		auto cend() const
		{
			return m_data.cend();
		}

		template <class... variadic>
		void insert(variadic&&... var_args)
		{
			insert(T(var_args...));
		}

		void insert(T&& element)
		{
			m_data.emplace(std::upper_bound(m_data.begin(), m_data.end(), element), std::move(element));
		}

	private:
		std::vector<T> m_data;
	};
}
