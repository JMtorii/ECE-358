#include <iostream>
#include <queue>
#include <random>

using namespace std;

/**
 * Output metrics to be returned after running the system
 */
double average_queue_packets = 0; // Average number of packets in the queue
double average_sojourn_time = 0; // Queueing delay + Service time
double idle_time = 0;
double proportion_idle = 0; // Proportion of ticks the server is idle
double probability_packet_loss = 0; // Probability of packet loss (only relevant for M/D/1/K)

/**
 * Input parameters
 */
unsigned long tick_length = 0; // Simulation length (in ticks)
double avg_number_packets = 0; // Average packets per second to generate
unsigned long packet_length = 0; // Length of packets to send (in bits)
double transmission_rate = 0; // Transmission rate (bits per second)
double service_time = 0;
unsigned long queue_size = 0; // Queue capacity (for M/D/1/K queue)

queue<unsigned long> Queue;
unsigned long t_arrival, t_departure;

// Random distribution engine
default_random_engine generator;
uniform_real_distribution<double> distribution(0, 1);
unsigned long next_send_tick = 0;

// C++ 11 method of uniform distribution
void update_next_tick() {
	double u = distribution(generator);
	next_send_tick = (-1.0/avg_number_packets)*log(1 - u);
}

/* Generate a packet as per the exponential distribution and insert the
packet in the queue (an array or a linked list)*/
void arrival(int t)
{
	if (t == next_send_tick) {
		// TODO: send packet
		
		update_next_tick();
	}
}

/* Check the queue for the packet, if head of the queue is empty,
return 0 else if the queue is non-empty delete the packet from the
queue after an elapse of the deterministic service time. */
int departure(int t)
{
	if (Queue.empty()) {
		idle_time++;
		return 0;
	} else {
		t_departure--;

		if (t_departure < 0) {
			unsigned long sojourn_time = t - Queue.front() + service_time;
			average_sojourn_time = average_sojourn_time * ((tick_length - 1) / tick_length) + (sojourn_time / tick_length);
			t_departure = service_time;
			Queue.pop();
		}

		return 1;
	}
}

void start_simulation(int ticks)
{
	for (int t = 1; t <= ticks; ++t)
	{
		arrival(t); /* call the arrival procedure*/
		departure(t); /*call the departure procedure*/
		average_queue_packets = average_queue_packets * ((ticks - 1) / ticks) + (Queue.size() / ticks);
	}
}

/*Calculate and display the results such as average number of packets
in queue, average delay in queue and idle time for the server. */
void compute_performances()
{
	
	double Pidle = idle_time / tick_length;

}

/** 
 * Initialize important terms such as t_arrival = exponential r.v, # of
 * pkts in queue = 0, t_departure = t_arrival ( this implies that first
 * time departure will be called as soon as a packet arrives in the
 * queue
 */
int main()
{
	int mode = 0;
	while (mode != 1 && mode != 2) {
		cout << "Which type of queue would you like to simulate?  Select 1 for M/D/1 or 2 for M/D/1/K: ";
		cin >> mode;
		cout << endl;
		if (mode != 1 && mode != 2) {
			cout << "Please enter '1' or '2'." << endl;
		}
	}

	if (mode == 1) {
		queue_size = -1;
	}
	else {
		cout << "Capacity of queue: ";
		cin >> queue_size;
		cout << endl;
	}

	cout << "Length of simulation, in ticks: ";
	cin >> tick_length;
	cout << endl;

	cout << "Average number of packets per second generated/arrived: ";
	cin >> avg_number_packets;
	cout << endl;

	cout << "Length of packet: ";
	cin >> packet_length;
	cout << endl;

	cout << "Transmission rate: ";
	cin >> transmission_rate;
	cout << endl;

	service_time = packet_length / transmission_rate;

	// Start off by determining when to send the first packet
	update_next_tick();

	// Run simulation
	start_simulation(tick_length);

	// Process and display results
	compute_performances();

	return 0;
}
