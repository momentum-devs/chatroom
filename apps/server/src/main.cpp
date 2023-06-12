#include <iostream>
#include <memory>
#include <odb/transaction.hxx>

#include "Person.odb.hpp"
#include "Person.hpp"
#include "Database.hpp"


int main()
{
	try
	{
		std::auto_ptr<odb::database> db( new Database( "test.db" ) );

		std::string john_id, jane_id, joe_id;
		{
			Person john( "John", Date(1969,1,1) );
			Person jane( "Jane", Date(1969,2,4) );
			Person joe( "Joe", Date(1969,3,9) );

			odb::transaction t( db->begin() );

			john_id = db->persist( john );
			jane_id = db->persist( jane );
			joe_id = db->persist( joe );

			t.commit();
		}
	}
	catch( const odb::exception & e )
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
