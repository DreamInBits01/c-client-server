[Workers]
-Create threads pool
-Create a mock worker
-Pool should contain logic about threads pool
-Worker should be the function used by the threads

[Connections manager]
-Should use API provided by the workers to use the pool
-Should store a refrence to a pool
-Should provide the pool to clients for disptaching (Via handler_context inside the connection)

[Event loop]
-Should handle timeouts
-Should close connected sockets if there request_start time is above 10 seconds and their state is processing
-Should create an event_loop context that has a connections manager, a threadpool, and a daemon
[IO]
-Should handle partial reads
-Should handle partial sends
