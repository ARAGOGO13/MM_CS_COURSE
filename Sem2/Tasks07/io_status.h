#define EPS 1e-16

typedef enum io_status_ {
	SUCCESS,
	ERROR_OPEN,
	ERROR_READ,
	ERROR_MEM,

} io_status;

typedef enum task_status_ {
	SUCCESS_,
	ERROR_MATRIX, 
	ERROR_VECTOR
} task_status;
