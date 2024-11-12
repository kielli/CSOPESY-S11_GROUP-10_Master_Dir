#include "Scheduler.h"

void Scheduler::assignProcessToCore(CPU& core) {
	if (!this->processList.empty()) {
		core.assignProcess(this->processList.front());
		popProcess();
	}
}

void Scheduler::popProcess() {
	processList.erase(processList.begin());
}

void Scheduler::stopScheduler() {
	stopExecution = true;

	for (auto& t : this->coreThreads) {
		if (t.joinable()) {
			t.join();
		}
	}
}

bool Scheduler::allCoresIdle() const {
	for (const auto& core : this->cpuList) {
		if (core.isCoreWorking()) {
			return false;
		}
	}

	return true;
}

vector<CPU>& Scheduler::get_cpuList() {
	return this->cpuList;
}

vector<Process>& Scheduler::getProcessList() {
	return this->processList;
}

vector<Scheduler::FinishedProcess> Scheduler::get_finishedProcess() {
	return this->finishedProcesses;
}