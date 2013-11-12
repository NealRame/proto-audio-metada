#include <exception>

struct ID3Error : public std::exception {
	enum Status {
		NoTag,
		BadFrame,
		UnsupportedVersion,
	};

	explicit ID3Error (Status status) throw() :
		status(status) {
	}

	const Status status;
};