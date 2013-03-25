SimpleOrm
=========

simple data Orm based on PDO

# Installing/Configuring
```
phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make && make install
```
Then add
```
extension="SimpleOrm.so"
```
to your php.ini

#Usage

###SimpleOrm::getInstance(PDO $pdo)
```php
<?php
$dsn = "mysql:host=localhost;dbname=sakila";
$pdo= new PDO($dsn, 'root', '');
$obj=SimpleOrm::getInstance($pdo);
```
###SimpleOrm::query(string $query)
```php
<?php
$obj->query('select * from actor');
```

###SimpleOrm::exec(string $query)
```php
<?php
//excute a query
$obj->exec('insert into actor (id, name, age)');
```

###SimpleOrm::select(string $table)
```php
<?php
//chose a table
$obj->select('actor');
```

###SimpleOrm::find(mixed)
```php
<?php
//find all
$obj->select('actor')->find();

//find record where id==2
$obj->select('actor')->find(2);

//find record where id in [1,2,4,5,6]
$obj->select('actor')->find([1,2,4,5,6]);
```

###SimpleOrm::field(string $columns)
```php
<?php
//select columns 
$obj->select('actor')->field('actor_id,first_name,last_name');
```

###SimpleOrm::where()
```php
<?php
//set where condition 
$obj->select('actor')->field('actor_id,first_name,last_name')->where("where last_name='GABLE'");
```