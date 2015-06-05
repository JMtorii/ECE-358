#include <iostream>
#include <queue>
#include <random>
#include <time.h>

using namespace std;

/**
 * Output metrics to be returned after running the system
 */
double average_queue_packets = 0; // Average number of packets in the queue
double average_sojourn_time = 0; // Queueing delay + Service time
double idle_time = 0;
double proportion_idle = 0; // Proportion of ticks the server is idle
unsigned long long packets_sent = 0;
unsigned long long packets_received = 0;

/**
 * Input parameters
 */
int mode = 0; // Mode (equals "2" if queue has a limited length, "1" for normal)
unsigned long long tick_length = 0; // Simulation length (in ticks)
double avg_number_packets = 0; // Average packets per second to generate
unsigned long long packet_length = 0; // Length of packets to send (in bits)
double transmission_rate= 0; // Transmission rate (bits per second)
double service_time = 0;
unsigned long long max_queue_size = 0; // Queue capacity (for M/D/1/K queue)

queue<unsigned long long> Queue;
unsigned long long t_arrival = 0, t_departure = 0;

// Random distribution engine
default_random_engine generator((unsigned int)time(0));
uniform_real_distribution<double> distribution(0, 1);

// C++ 11 method of uniform distribution
void update_next_tick(unsigned long long t) {
	double u = distribution(generator);
	t_arrival = ((double)(t)) + ((-1.0/avg_number_packets)*log(1.0 - u) * pow(10, 6));
	//cout << "T_ARRIVAL: " << t_arrival << endl;
}

/**
 * Generate a packet as per the exponential distribution and insert the
 * packet in the queue (an array or a linked list)
 */
void arrival(unsigned long long t)
{
	if (t == t_arrival) {
		// Send the packet
		packets_sent++;

		if (mode == 2 && Queue.size() >= max_queue_size) {
			// do nothing
		}
		else {
			Queue.push(t);
		}

		// Update t_arrival
		update_next_tick(t);
	}
}

/**
 * Check the queue for the packet, if head of the queue is empty,
 * return 0 else if the queue is non-empty delete the packet from the
 * queue after an elapse of the deterministic service time.
 */
int departure(unsigned long long t)
{
	if (Queue.empty()) {
		idle_time++;
		t_departure = t + service_time; // Not allowed to process packets we haven't received
		return 0;
	} else {
		if (t >= t_departure) {
			unsigned long long sojourn_time = t - Queue.front();
			packets_received++;
			average_sojourn_time = average_sojourn_time * (((double)packets_received - 1) / (double)packets_received) + ((double)sojourn_time / packets_received);
			t_departure = t + service_time;
			//cout << "T_DEPARTURE: " << t_departure << endl;
			Queue.pop();
		}

		return 1;
	}
}

void start_simulation(unsigned long long ticks)
{
	for (int t = 1; t <= ticks; ++t)
	{
		arrival(t); /* call the arrival procedure*/
		departure(t); /*call the departure procedure*/
		average_queue_packets = (double)average_queue_packets * (((double)ticks - 1) / (double)ticks) + ((double)Queue.size() / (double)ticks);
	}
}

/**
 * Calculate and display the results such as average number of packets
 * in queue, average delay in queue and idle time for the server.
 */
void compute_performances()
{
	double Pidle = idle_time / tick_length;
	cout << "Proportion of ticks server idle: " << Pidle << endl;
	cout << "Average number of packets in queue: " << average_queue_packets << endl;
	cout << "Average sojourn time: " << average_sojourn_time << endl;

	if (mode == 2) {
		if (packets_sent == 0) {
			cout << "No packets received." << endl;
		}
		else {
			cout << "Packets sent: " << packets_sent << endl;
			cout << "Packets received: " << packets_received << endl;
			cout << "Probability of packet loss: " << ((double)packets_sent - (double)packets_received) / ((double)packets_sent) << endl;
		}
	}
}

/**
 * Initialize important terms such as t_arrival = exponential r.v, # of
 * pkts in queue = 0, t_departure = t_arrival ( this implies that first
 * time departure will be called as soon as a packet arrives in the
 * queue
 */
int main()
{
    cout << "sizeOf ulong: " << sizeof(unsigned long) << endl;
    cout << "sizeOf ulonglong: " << sizeof(unsigned long long) << endl;
	while (mode != 1 && mode != 2) {
		cout << "Which type of queue would you like to simulate?  Select 1 for M/D/1 or 2 for M/D/1/K: ";
		cin >> mode;
		cout << endl;
		if (mode != 1 && mode != 2) {
			cout << "Please enter '1' or '2'." << endl;
		}
	}

	if (mode == 2) {
		cout << "Capacity of queue: ";
		cin >> max_queue_size;
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

	// Mbits/second is equivalent to bits/microsecond
	cout << "Transmission rate (Mbits/second): ";
	cin >> transmission_rate;
	cout << endl;

	service_time = packet_length / transmission_rate;
	cout << "Service time is: " << service_time << " bits per microsecond" << endl << endl;
	cout << "============================================" << endl;

	// Start off by determining when to send the first packet
	update_next_tick(0);

	// First departure is equal to t_arrival
	t_departure = t_arrival;

	// Run simulation
	start_simulation(tick_length);

	// Process and display results
	compute_performances();

	cout << "Press Enter to Continue";
	cin.ignore();
	cin.ignore();

	return 0;
}
