/**
 * This code uses the RDMA library to create a new connection, bind it to a local address and port,listens for incoming connections,
 * and accept an incoming connection. The example also shows how to create a new queue pair, register a memory region, and use RDMA 
 * write operation to transfer data over the network. 
 * 
 * It's important to note that this is a simple example and in a real-world application, you would need to handle errors, timeouts, and
 * disconnections. Also, you may need to adjust the code to fit your specific use case, such as handling different data types or implementing 
 * more advanced communication patterns.
 *
 * It's also important to note that this code is using the librdmacm library, this library is a user-space library that provides 
 * communication management services to the verbs API, it's not built-in to the operating system and it needs to be installed before using it.


 **/



#include <iostream>
#include <rdma/rdma_cma.h>

using namespace std;

// Structure for storing connection information
struct connection {
    struct rdma_cm_id *id;
    struct ibv_qp *qp;
    struct ibv_mr *mr;
    char *buf;
};

// Function for creating a new connection
int connect(struct connection *conn) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(2000);
    sin.sin_addr.s_addr = INADDR_ANY;

    // Create a new RDMA CM ID
    struct rdma_cm_id *id;
    if (rdma_create_id(NULL, &id, NULL, RDMA_PS_TCP)) {
        cerr << "Error creating RDMA CM ID" << endl;
        return -1;
    }

    // Bind the new ID to a local address and port
    if (rdma_bind_addr(id, (struct sockaddr *)&sin)) {
        cerr << "Error binding address and port" << endl;
        return -1;
    }

    // Listen for incoming connections
    if (rdma_listen(id, 10)) {
        cerr << "Error listening for incoming connections" << endl;
        return -1;
    }

    // Accept an incoming connection
    if (rdma_accept(id, NULL)) {
        cerr << "Error accepting incoming connection" << endl;
        return -1;
    }

    // Create a new queue pair
    struct ibv_qp_init_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.cap.max_send_wr = attr.cap.max_recv_wr = 10;
    attr.cap.max_send_sge = attr.cap.max_recv_sge = 1;
    attr.cap.max_inline_data = 0;
    attr.qp_type = IBV_QPT_RC;

    if (rdma_create_qp(id, NULL, &attr)) {
        cerr << "Error creating queue pair" << endl;
        return -1;
    }

    // Register a memory region
    struct ibv_mr *mr;
    conn->buf = new char[sizeof(int)];
    mr = ibv_reg_mr(id->pd, conn->buf, sizeof(int), IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);
    if (!mr) {
        cerr << "Error registering memory region" << endl;
        return -1;
    }

    // Store the connection information
    conn->id = id;
    conn->qp = id->qp;
    conn->mr = mr;

    return 0;
}

// Function for sending data
int send_data(struct
connection *conn, int data) {
// Copy the data to the shared buffer
memcpy(conn->buf, &data, sizeof(int));
// Create a new scatter/gather element for the memory region
struct ibv_sge sge;
memset(&sge, 0, sizeof(sge));
sge.addr = (uintptr_t)conn->buf;
sge.length = sizeof(int);
sge.lkey = conn->mr->lkey;

// Create a new work request for the send operation
struct ibv_send_wr wr, *bad_wr = NULL;
memset(&wr, 0, sizeof(wr));
wr.wr_id = (uintptr_t)conn;
wr.sg_list = &sge;
wr.num_sge = 1;
wr.opcode = IBV_WR_RDMA_WRITE;
wr.send_flags = IBV_SEND_SIGNALED;
wr.wr.rdma.remote_addr = (uintptr_t)conn->buf;
wr.wr.rdma.rkey = conn->mr->rkey;

// Post the work request to the queue pair
if (ibv_post_send(conn->qp, &wr, &bad_wr)) {
    cerr << "Error posting send request" << endl;
    return -1;
}

// Wait for the send operation to complete
struct ibv_wc wc;
if (ibv_poll_cq(conn->id->send_cq, 1, &wc) < 1) {
    cerr << "Error polling completion queue" << endl;
    return -1;
}
if (wc.status != IBV_WC_SUCCESS) {
    cerr << "Send completion with error status: " << wc.status << endl;
    return -1;
}

return 0;
