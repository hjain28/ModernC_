#include <exception>

struct empty_vector: std::exception{
	const char* what() const throw();
};

template<typename T>
class threadsafe_vector {
	private:
		std::vector<T> data;
		mutable std::mutex m;
	public:
		threadsafe_vector(){}
		threadsafe_vector(const threadsafe_vector& other) {
			std::lock_guard<std::mutex> lock(other.m);
			data=other.data;
		}
		threadsafe_vector& operator=(const threadsafe_vector&) = delete;
		void push(T new_value){
			std::lock_guard<std::mutex> lock(m);
			data.push(std::move(new_value));
		}
		std::shared_ptr<T> pop(){
			std::lock_guard<std::mutex> lock(m);
			if(data.empty()) throw empty_vector();
			std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
			data.pop();
			return res;
		}
		void pop(T& value){
			std::lock_guard<std::mutex> lock(m);
			if(data.empty()) throw empty_vector();
			value=std::move(data.top());
			data.pop();
		}
		bool empty() const {
			std::lock_guard<std::mutex> lock(m);
			return data.empty();
		}
};