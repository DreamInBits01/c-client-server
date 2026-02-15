[Workers]
-Create threads pool
-Create a mock worker
-Pool should contain logic about threads pool
-Worker should be the function used by the threads

[Connections manager]
-Should use API provided by the workers to use the pool:DONE
-Should provide the pool to clients for disptaching (Via handler_context inside the connection):DONE

[Event loop]
-Should handle timeouts
-Should create an event_loop context that has a connections manager, a threadpool, and a daemon:DONE
[IO]
-Should handle partial reads:DONE
-Should handle partial sends:DONE
[Daemon]
-Write check_health handler:DONE
