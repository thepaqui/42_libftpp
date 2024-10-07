#ifndef LIBFTPP_SINGLETON_HPP
# define LIBFTPP_SINGLETON_HPP

# include <memory>
# include <exception>

template <typename TType>
class Singleton {
private :
	static std::unique_ptr<TType>	obj;

protected :
	Singleton() = default;
	virtual ~Singleton() = default;

public :
//	Singleton(const Singleton&) = delete; // Prevent copy constructor
//	Singleton& operator=(const Singleton&) = delete; // Prevent copy assignment

	static TType*	instance();

	template <typename ... TArgs>
	static void	instantiate(TArgs&&... p_args);

	/* Exceptions */
	// TODO: See if some standard exceptions might fit better
	class NoInstanceException : public std::exception {
	public :
		const char*	what() const noexcept { return "Singleton: Instance not yet created."; }
	};

	class AlreadyAnInstanceException : public std::exception {
	public :
		const char*	what() const noexcept { return "Singleton: Instance already created."; }
	};
};

template <typename TType>
std::unique_ptr<TType> Singleton<TType>::obj = nullptr;

# include "singleton.tpp"

#endif