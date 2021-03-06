/*
 * Swift Parallel Scripting Language (http://swift-lang.org)
 * Code from Java CoG Kit Project (see notice below) with modifications.
 *
 * Copyright 2005-2014 University of Chicago
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//----------------------------------------------------------------------
//This code is developed as part of the Java CoG Kit project
//The terms of the license can be found at http://www.cogkit.org/license
//This message may not be removed or altered.
//----------------------------------------------------------------------

/*
 * Created on Jun 29, 2014
 */
package org.griphyn.vdl.karajan.monitor.monitors.http;

import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.SortedSet;

import org.griphyn.vdl.karajan.monitor.items.ApplicationItem;
import org.griphyn.vdl.karajan.monitor.items.ApplicationState;

public class AppsSummaryBuilder {

    private final SortedMap<String, List<SortedSet<ApplicationItem>>> byName;
    private BrowserDataBuilder db;

    public AppsSummaryBuilder(BrowserDataBuilder db) {
        this.byName = db.getByName();
        this.db = db;
    }

    public void getData(JSONEncoder e) {
        // counts of each state by name
        e.beginMap();
          db.writeEnabledStates(e, "enabledStates");
          e.writeMapKey("apps");
          e.beginMap();
          for (Map.Entry<String, List<SortedSet<ApplicationItem>>> en : byName.entrySet()) {
              e.writeMapKey(en.getKey());
              e.beginArray();
                int[] counts = new int[ApplicationState.values().length];
                for (ApplicationState s : ApplicationState.values()) {
                    int stateIndex = s.getAliasIndex();
                    counts[stateIndex] += en.getValue().get(s.ordinal()).size();
                }
                for (ApplicationState s : ApplicationState.values()) {
                    if (s.isEnabled()) {
                        e.beginArrayItem();
                          e.beginArray();
                            e.writeArrayItem(s.ordinal());
                            e.writeArrayItem(counts[s.ordinal()]);
                          e.endArray();
                        e.endArrayItem();
                    }
                }
              e.endArray();
          }
          e.endMap();
        e.endMap();
    }
}
