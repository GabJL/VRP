#ifndef _CINSTANCE_VRP_
#define _CINSTANCE_VRP_

#include <string>
#include <vector>

using namespace std;

class cInstance
{
	public:
		cInstance(string filename);

		int nCustomers() const;
		int capacity() const;
		double maxRouteTime() const;
		double distance(const int i, const int j) const;
		int demand(const int i) const;
		int service_time() const;
		double best_cost() const;

	private:
		void genDistances(const vector<int> &x,const vector<int> &y);

		int _nCustomers;
		int _capacity;
		vector<vector<double> > _distance;
		int _service_time;
		double _maxRouteTime;
		double _best_cost;
		vector<int> _demand;
  };

#endif
