# Generated snapshots

New generated-output cases use `test/snapshot/<family>/<case>/case.json` and
reviewable expected artifacts. Existing expected trees remain registered in
place to avoid churn. See [`docs/testing.md`](../../docs/testing.md).

Update only through `npm run snapshots:update -- --suite <suite-id>`; the
command prints semantic diffs before writing and CI never updates snapshots.
