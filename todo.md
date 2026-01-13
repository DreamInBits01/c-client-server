- Create a wrapper struct (has last_time and a struct pollfd) for storing connections and their last activity time
  called (Connection)
- Copy the fds from the connections array into a temp array to monitor with poll
- Create a thread to delete connection that has been stale for certian time period seconds
