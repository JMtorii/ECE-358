
using namespace std;

void main()
{
	/*Initialise important terms such as t_arrival = exponential r.v, # of
	pkts in queue = 0, t_departure = t_arrival ( this implies that first
	time departure will be called as soon as a packet arrives in the
	queue*/
	int ticks = 0;

	start_simulation(ticks);
	compute_performances();
}

void start_simulation(int ticks)
{
	for (unsigned int t = 1; t <= ticks; ++t)
	{
		arrival(t); /* call the arrival procedure*/
		departure(t); /*call the departure procedure*/
	}
}

void arrival(int t)
{
	/* Generate a packet as per the exponential distribution and insert the
	packet in the queue (an array or a linked list)*/
}

void departure(int t)
{
	/* Check the queue for the packet, if head of the queue is empty,
	return 0 else if the queue is non-empty delete the packet from the
	queue after an elapse of the deterministic service time. */
}

void compute_performances()
{
	/*Calculate and display the results such as average number of packets
	in queue, average delay in queue and idle time for the server. */
}