// Struct Job_Complete: {String word, int found, worker}
// 
// Main Thread:
//     Setup:
//         Create Circular Buffer using buf from p_thread library.
//         Create Pool of Five Threads.
//         Create Logger Thread.
//         Load dictionary from file.
//         Initialize waiting_jobs queue of file descriptors of requests.
//         Open Socket.
//
//     Main:
//         Busy wait on socket.
//         Block on fd queue full.
//         When job comes in, put fd in queue.
//         Signal job in fd queue.
//
//
// Worker Threads:
//     Block on fd queue empty.
//     When signaled, take a client from queue and service.
//     Signal queue not full.
//
//     Spell Check service:
//         Read word from client.
//         Check dictionary for word.
//         Return true of false.
//         If client terminates connection, close.
//
// Logger Thread
//     Block on Jobs complete queue full.
//     When jobs complete queue not empty:
//         Get job from queue.
//         Add to log file.
