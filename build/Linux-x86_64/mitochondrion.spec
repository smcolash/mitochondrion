Summary: A platform for development and deployment of model-based software.
Name: Mitochondrion
Version: 2.0
Release: 1
Copyright: GPL
#Source:%{name}-%{version}.tar.gz
Group: Development/Libraries
BuildRoot:/var/tmp/%{name}

%description
A platform for development and deplyment of model-based software in terms of
actors with internal hierarchical state machines that communicate using queued
messages over well defined ports. Additional run-time support includes browser
(http) access to internal trace buffers and visualization of that trace data
as message sequence charts and dynamic association diagrams.

%prep
mkdir -p $RPM_BUILD_ROOT/SOURCES
cp -rfp ../include $RPM_BUILD_ROOT/SOURCES
cp -rfp ../source $RPM_BUILD_ROOT/SOURCES

# %setup -q

%build
make

%install
make 
install -D MitoLog/include/mitochondrion/Log.h $RPM_BUILD_ROOT/usr/include/mitochondrion/Log.h
install -D MitoLog/include/mitochondrion/WebServer.h $RPM_BUILD_ROOT/usr/include/mitochondrion/WebServer.h
install -D MitoLog/libMitoLog.a  $RPM_BUILD_ROOT/usr/lib/libMitoLog.a
install -D MitoLog/libMitoLog.so  $RPM_BUILD_ROOT/usr/lib/libMitoLog.so

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

/usr/bin/zzz

%changelog
* Thu Mar 08 2012 Scott McOlash
- Added this comment.
- Removed older comments.

