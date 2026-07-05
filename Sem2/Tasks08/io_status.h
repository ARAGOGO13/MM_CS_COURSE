#define EPS 1e-15
#define IJ_EL(i, j) ((i) * (n) + (j))


typedef enum io_status_ {
	SUCCESS,
	ERROR_OPEN,
	ERROR_READ,
	ERROR_MEM,

} io_status;
