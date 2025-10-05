/**
 *
 * @file interrupts.cpp
 * @author Adam Rebello  and Marc Aoun
 *
 */

#include "interrupts.hpp"
#include <fstream>
#include <string>



int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output 

    /******************ADD YOUR VARIABLES HERE*************************/
    unsigned long long current_time = 0; //Simulation time clock in CPU cycles
    const int CONTEXT_SAVE_TIME = 3; //Time constants for overhead 
    const int CONTEXT_RESTORE_TIME = 2;
    const int IRET_TIME = 1;


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            execution += "Time " + std::to_string(current_time) + ": CPU is running for " + std::to_string(duration) + " cycles\n";
            current_time += duration;
        }
        else if (activity == "syscall") {
            execution += "Time " + std::to_string(current_time) + ": Recieved System call interruprt\n";

            execution += "Time " + std::to_string(current_time) + ": Saving context\n";
            current_time += CONTEXT_SAVE_TIME;

            std::string isr_address = (intr_num >= 0 && intr_num < (int)vectors.size()) ? vectors[intr_num] : "ISR Unknown";
            execution += "Time " + std::to_string(current_time) + ": Jump to ISR at address " + isr_address + "\n";

            int isr_delay = (intr_num >= 0 && intr_num < (int)delays.size()) ? delays[intr_num] : 0;
            execution += "Time " + std::to_string(current_time) + ": ISR is executing for " + std::to_string(isr_delay) + " cycles\n";
            current_time += isr_delay;

            execution += "Time " + std::to_string(current_time) + ": Restoring context\n";
            current_time += CONTEXT_RESTORE_TIME;

            execution += "Time " + std::to_string(current_time) + ": Executing IRET\n";
            current_time += IRET_TIME;
        }
        else if (activity.rfind("device", 0) == 0) {
            execution += "Time " + std::to_string(current_time) + ": Device interrupt " + std::to_string(intr_num) + " received\n";

            execution += "Time " + std::to_string(current_time) + ": Saving the context\n";
            current_time += CONTEXT_SAVE_TIME;

            std::string isr_address = (intr_num >= 0 && intr_num < (int)vectors.size()) ? vectors[intr_num] : "ISR Unknown";
            execution += "Time " + std::to_string(current_time) + ": Jump to ISR at address " + isr_address + "\n";

            int isr_delay = (intr_num >= 0 && intr_num < (int)delays.size()) ? delays[intr_num] : 0;
            execution += "Time " + std::to_string(current_time) + ": ISR is executing for " + std::to_string(isr_delay) + " cycles\n";
            current_time += isr_delay;

            execution += "Time " + std::to_string(current_time) + ": Restoring context\n";
            current_time += CONTEXT_RESTORE_TIME;

            execution += "Time " + std::to_string(current_time) + ": Executing IRET\n";
            current_time += IRET_TIME;
        }
        else {
            execution += "Time " + std::to_string(current_time) + ": Unknown activity '" + activity + "' - ignored\n";
        }



        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
