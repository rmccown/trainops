========================================================================
       TrainOps
========================================================================



/////////////////////////////////////////////////////////////////////////////

Current DB Schema:

<?xml version="1.0" encoding="iso-8859-1"?>
<!-- TrainOps database schema 
    (c) 2006 - 2007 Bob McCown
    www.largescalecentral.com
-->
<dbschema>
    <Commands>
    </Commands>
    <table name="_Config">
        <field name="id" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="rrname" type="VARCHAR (255) NULL"/>
        <field name="reporting_marks" type="VARCHAR (24) NULL"/>
        <field name="logo" type="VARCHAR (255) NULL"/>
        <field name="date" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="rrdate" type="VARCHAR (30) NULL"/>
        <field name="LoadEmptyDays" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="car_length" type="INTEGER UNSIGNED NOT NULL DEFAULT 15"/>
        <field name="car_method" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="offlayouttime" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="realistic_reports" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
    </table>
    <table name="Caboose">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="reporting_marks" type="VARCHAR(40) NULL DEFAULT ''"/>
        <field name="car_number" type="VARCHAR(20) NULL DEFAULT '0'"/>
        <field name="car_length" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Trains_Scheduled_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Location_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
    </table>
    <table name="Cars">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="CarTypes_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Commodities_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Location_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Siding_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Trains_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Industry_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="reporting_marks" type="VARCHAR(40) NULL DEFAULT ''"/>
        <field name="car_number" type="VARCHAR(20) NULL DEFAULT '0'"/>
        <field name="car_length" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="delivery_date" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="loadempty" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="offlayout" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="tonnage" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
    </table>
    <table name="Cars_Not_Found">
        <field name="CarTypes_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Trains_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Industries_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="reverse" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="train_date" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
    </table>
    <table name="CarTypes">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="type_id" type="VARCHAR(20) NULL"/>
        <field name="description" type="VARCHAR(255) NULL"/>
        <field name="active" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="passenger" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Commodities">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="name" type="VARCHAR(255) NULL"/>
    </table>
    <table name="Commodities_Cars">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="CarTypes_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="Commodities_FK" type="INTEGER UNSIGNED NOT NULL"/>
    </table>
    <table name="Industries">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Sidings_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="name" type="VARCHAR(255) NULL"/>
        <field name="notes" type="VARCHAR(255) NULL"/>
        <field name="sort_order" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Industries_Cars">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Industries_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="CarTypes_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="InOut" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="LoadEmptyDays" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="Quantity_low" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="Quantity_high" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="Quantity_percentage" type="INTEGER UNSIGNED NOT NULL DEFAULT 100"/>
        <field name="LastDelivery" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Industries_Commodities">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Industries_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="Commodities_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="InOut" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="LoadEmptyDays" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="Quantity_low" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="Quantity_high" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="Quantity_percentage" type="INTEGER UNSIGNED NOT NULL DEFAULT 100"/>
        <field name="LastDelivery" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="LoadsEmpties">
        <field name="id" type="INTEGER PRIMARY KEY"/>
    </table>
    <table name="Locations">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="name" type="VARCHAR(255) NULL"/>
        <field name="notes" type="VARCHAR(255) NULL"/>
        <field name="sort_order" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Locomotives">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="active" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="Locations_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="car_number" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="road_name" type="VARCHAR(255) NOT NULL"/>
        <field name="capacity" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="description" type="VARCHAR(255) NULL"/>
    </table>
    <table name="Sidings">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Locations_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="name" type="VARCHAR(255) NULL"/>
        <field name="capacity" type="INTEGER UNSIGNED NOT NULL DEFAULT 1"/>
        <field name="ends" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="direction" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="yard" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="getempties" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="sort_order" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Trains">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="trainid" type="VARCHAR(4) NULL"/>
        <field name="name" type="VARCHAR(255) NULL"/>
    </table>
    <table name="Trains_Location">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Train_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="Location_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="route_order" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="localswitcher" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Trains_Industry">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Train_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="Location_FK" type="INTEGER UNSIGNED NOT NULL"/>
        <field name="Industry_FK" type="INTEGER UNSIGNED NOT NULL"/>
    </table>
    <table name="Trains_Scheduled">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Trains_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Locomotive_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Caboose_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="run_date" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="train_run" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Trains_Scheduled_Cars">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Trains_Scheduled_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Cars_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Industry_from_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Industry_to_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Location_from_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Location_to_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Siding_from_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Siding_to_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Commodities_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="loadempty" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
        <field name="offlayout" type="INTEGER UNSIGNED NOT NULL DEFAULT 0"/>
    </table>
    <table name="Trains_Scheduled_Locations">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Trains_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Location_FK" type="INTEGER UNSIGNED NOT NULL"/>
    </table>
    <table name="Trains_Scheduled_Locations_Cars">
        <field name="id" type="INTEGER PRIMARY KEY"/>
        <field name="Trains_Scheduled_Locations_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
        <field name="Cars_FK" type="INTEGER UNSIGNED NOT NULL DEFAULT -1"/>
    </table>
</dbschema>



/////////////////////////////////////////////////////////////////////////////
