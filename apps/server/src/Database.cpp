#include "Database.hpp"


#include <odb/schema-catalog.hxx>
#include <iostream>


Database::Database( const std::string & name, bool clear, int flags, bool foreign_keys, const std::string & vfs, odb::details::transfer_ptr<odb::sqlite::connection_factory> transfer )
	: odb::sqlite::database( name, flags, foreign_keys, vfs, transfer )
{
	if( !odb::schema_catalog::exists( *this ) )
		throw odb::unknown_schema( name );
	try
	{
		odb::transaction t( this->begin() );
		odb::schema_catalog::create_schema( *this, "", clear );
		t.commit();
	}
	catch( const odb::exception & e )
	{
		// ok, schema seems to be existing already
		return;
	}
	std::cerr << "Created database schema in \"" << name << "\"\n";
}
