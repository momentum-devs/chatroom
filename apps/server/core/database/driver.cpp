#include <iostream>
#include <memory>
#include <odb/transaction.hxx>

#include "database.h"
#include "person.h"
#include "person_odb.h"

using namespace std;
using namespace odb::core;

int main()
{
    try
    {
        shared_ptr<database> db(create_database());

        unsigned long john_id, joe_id;

        {
            person john("John", "Doe", 33);
            person jane("Jane", "Doe", 32);
            person joe("Joe", "Dirt", 30);

            transaction t(db->begin());

            john_id = db->persist(john);
            db->persist(jane);
            joe_id = db->persist(joe);

            t.commit();
        }

        typedef odb::query<person> query;
        typedef odb::result<person> result;

        {
            transaction t(db->begin());

            result r(db->query<person>(query::age > 30));

            for (result::iterator i(r.begin()); i != r.end(); ++i)
            {
                cout << "Hello, " << i->first() << " " << i->last() << "!" << endl;
            }

            t.commit();
        }

        {
            transaction t(db->begin());

            unique_ptr<person> joe(db->load<person>(joe_id));
            joe->age(joe->age() + 1);
            db->update(*joe);

            t.commit();
        }

        {
            transaction t(db->begin());

            odb::result<person_stat> r(db->query<person_stat>());

            const person_stat& ps(*r.begin());

            cout << endl
                 << "count  : " << ps.count << endl
                 << "min age: " << ps.min_age << endl
                 << "max age: " << ps.max_age << endl;

            t.commit();
        }

        {
            transaction t(db->begin());
            db->erase<person>(john_id);
            t.commit();
        }
    }
    catch (const odb::exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
}
