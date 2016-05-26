import {bootstrap}    from '@angular/platform-browser-dynamic'
import { provideRouter, Routes } from '@ngrx/router';
import { LocationStrategy,HashLocationStrategy } from '@angular/common';
import { provide } from '@angular/core';
import {provideStore, combineReducers} from '@ngrx/store';
import {storeLogger} from "ngrx-store-logger";

import {AppComponent} from './components/app.component'
import {MqttComponent} from './components/mqtt/mqtt.component'
import {HelpComponent} from './components/help/help.component'
import {AboutComponent} from './components/about/about.component'
import {CounterComponent} from './components/counter/counter.component.ts'
import {PartyComponent} from './components/party/party.component';


import {counter} from './reducers/counter';
import {people} from './reducers/people';
import {filter} from './reducers/filter';
import {tmr} from './reducers/tmr';
import {status} from './reducers/status';



const routes: Routes = [
	{ path: '/', component: AboutComponent },
	{ path: '/about', component: AboutComponent },
	{ path: '/help', component: HelpComponent },
	{ path: '/cnt', component:CounterComponent},
	{ path: '/party', component: PartyComponent },
	{ path: '/mqtt', component: MqttComponent }
]

bootstrap(AppComponent, [
	provideRouter(routes),
	provide(LocationStrategy, { useClass: HashLocationStrategy }),
	provideStore(storeLogger()(combineReducers({ counter, people, filter, tmr, status })))
]);
// bootstrap(AppComponent, [
// 	provideRouter(routes)
// ]);
