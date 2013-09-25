/*
 * Copyright 2012 University of Chicago
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * Created on Jan 5, 2007
 */
package org.griphyn.vdl.karajan;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.globus.swift.catalog.TCEntry;
import org.globus.swift.catalog.TransformationCatalog;
import org.globus.swift.catalog.types.TCType;
import org.griphyn.vdl.util.FQN;

public class TCCache {
	private TransformationCatalog tc;
	private Map<Entry, List<TCEntry>> cache;
	private Entry entry;

	public TCCache(TransformationCatalog tc) {
		this.tc = tc;
		cache = new HashMap<Entry, List<TCEntry>>();
		entry = new Entry();
	}

	public synchronized List<TCEntry> getTCEntries(FQN tr, String host, TCType tctype) throws Exception {
		entry.set(tr, host, tctype);
		List<TCEntry> l = cache.get(entry);
		if (l == null && !cache.containsKey(entry)) {
			l = tc.getTCEntries(tr.getNamespace(), tr.getName(), tr.getVersion(), host, tctype);
			cache.put(new Entry(tr, host, tctype), l);
		}
		return l;
	}

	private class Entry {
		public FQN tr;
		public String host;
		public TCType tctype;

		public Entry() {
		}

		public Entry(FQN tr, String host, TCType tctype) {
			set(tr, host, tctype);
		}

		public void set(FQN tr, String host, TCType tctype) {
			this.tr = tr;
			this.host = host;
			this.tctype = tctype;
		}

		public boolean equals(Object obj) {
			if (obj instanceof Entry) {
				Entry other = (Entry) obj;
				return tr.equals(other.tr) && host.equals(other.host) && tctype.equals(other.tctype);
			}
			else {
				return false;
			}
		}

		public int hashCode() {
			return tr.hashCode() + host.hashCode() + tctype.hashCode();
		}
	}
}
