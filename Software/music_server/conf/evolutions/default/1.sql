# --- Created by Ebean DDL
# To stop Ebean DDL generation, remove this comment and start using Evolutions

# --- !Ups

create table album (
  id                        bigint auto_increment not null,
  name                      varchar(255),
  artist_id                 bigint,
  user_id                   bigint,
  constraint pk_album primary key (id))
;

create table artist (
  id                        bigint auto_increment not null,
  name                      varchar(255),
  user_id                   bigint,
  constraint pk_artist primary key (id))
;

create table configuration (
  id                        bigint auto_increment not null,
  udp_port                  integer,
  user_id                   bigint,
  constraint pk_configuration primary key (id))
;

create table device (
  id                        bigint auto_increment not null,
  user_id                   bigint,
  serial                    varchar(255),
  constraint pk_device primary key (id))
;

create table playlist (
  id                        bigint auto_increment not null,
  name                      varchar(255),
  user_id                   bigint,
  constraint pk_playlist primary key (id))
;

create table song (
  id                        bigint auto_increment not null,
  name                      varchar(255),
  path                      varchar(255),
  user_id                   bigint,
  artist_id                 bigint,
  album_id                  bigint,
  constraint pk_song primary key (id))
;

create table user (
  id                        bigint auto_increment not null,
  name                      varchar(255),
  surname                   varchar(255),
  email                     varchar(255),
  password                  varchar(255),
  repassword                varchar(255),
  constraint pk_user primary key (id))
;

alter table album add constraint fk_album_artist_1 foreign key (artist_id) references artist (id) on delete restrict on update restrict;
create index ix_album_artist_1 on album (artist_id);
alter table album add constraint fk_album_user_2 foreign key (user_id) references user (id) on delete restrict on update restrict;
create index ix_album_user_2 on album (user_id);
alter table artist add constraint fk_artist_user_3 foreign key (user_id) references user (id) on delete restrict on update restrict;
create index ix_artist_user_3 on artist (user_id);
alter table configuration add constraint fk_configuration_user_4 foreign key (user_id) references user (id) on delete restrict on update restrict;
create index ix_configuration_user_4 on configuration (user_id);
alter table device add constraint fk_device_user_5 foreign key (user_id) references user (id) on delete restrict on update restrict;
create index ix_device_user_5 on device (user_id);
alter table playlist add constraint fk_playlist_user_6 foreign key (user_id) references user (id) on delete restrict on update restrict;
create index ix_playlist_user_6 on playlist (user_id);
alter table song add constraint fk_song_user_7 foreign key (user_id) references user (id) on delete restrict on update restrict;
create index ix_song_user_7 on song (user_id);
alter table song add constraint fk_song_artist_8 foreign key (artist_id) references artist (id) on delete restrict on update restrict;
create index ix_song_artist_8 on song (artist_id);
alter table song add constraint fk_song_album_9 foreign key (album_id) references album (id) on delete restrict on update restrict;
create index ix_song_album_9 on song (album_id);



# --- !Downs

SET FOREIGN_KEY_CHECKS=0;

drop table album;

drop table artist;

drop table configuration;

drop table device;

drop table playlist;

drop table song;

drop table user;

SET FOREIGN_KEY_CHECKS=1;

