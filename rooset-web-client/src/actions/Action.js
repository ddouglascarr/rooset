// @flow

import type { PageAction } from "./PageAction";
import type { QueryAction } from "./http/QueryActions";

export type HttpAction = QueryAction; // | CommandAction;
export type Action = PageAction | QueryAction;
